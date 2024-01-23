#include "SoundHandler.h"

#include <spdlog/spdlog.h>

#include "DevicePresets.h"
#include "Midi1Output.h"
#include "ParameterData.h"
#include "SoundMidiInMsgHandler.h"
#include "SoundMidiOutMsgHandler.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound;

SoundHandler::SoundHandler(
    std::string deviceName, const description::sound::Section& rSoundSection,
    std::shared_ptr<preset::DevicePresets> soundPresets,
    std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept :
    m_deviceName(std::move(deviceName)),
    m_rSoundSection(rSoundSection),
    m_paramStorage(rSoundSection),
    m_presetHandler(rSoundSection, m_paramStorage, std::move(soundPresets),
                    std::move(pActualPresetNames)),
    m_arpeggiators(m_rSoundSection.voices.size())
{
   for (int voiceIdx = 0; voiceIdx < m_arpeggiators.size(); ++voiceIdx)
   {
      m_arpeggiators[voiceIdx].onNoteOn(
          [this, voiceIdx](int note, float velocity) {
             assert(m_midiOutHandler);
             m_midiOutHandler->noteOn(voiceIdx, note, velocity);
          });
      m_arpeggiators[voiceIdx].onNoteOff(
          [this, voiceIdx](int note, float velocity) {
             assert(m_midiOutHandler);
             m_midiOutHandler->noteOff(voiceIdx, note, velocity);
          });
      m_arpeggiators[voiceIdx].setRange(arp::RangeType::Octave, 1);
   }
   m_paramStorage.onActualChanged(
       [this](int voiceIdx, int paramIdx, float oldVal, float newVal) {
          if (const auto& paramDescr =
                  m_rSoundSection.parameterDescription(voiceIdx, paramIdx);
              paramDescr.role ==
              description::sound::Parameter::Role::ComponentSelector)
          {
             handleComponentSelectorParamChange(paramDescr, voiceIdx, oldVal,
                                                newVal);
          }
          m_midiOutHandler->sendSoundParameter(voiceIdx, paramIdx, newVal);
       });
   m_paramStorage.onActualPresetChanged(
       [this](int voiceIdx, const std::string& presetName) {
          emitActualPresetChanged(voiceIdx, presetName);
       });
}

void SoundHandler::handleComponentSelectorParamChange(
    const description::sound::Parameter& paramDescr, int voiceIdx, float oldVal,
    float newVal)
{
   assert(paramDescr.source.midi->sourceRanges);
   const std::string compNamePrev =
       (-1 == oldVal)
           ? ""
           : paramDescr.source.midi->sourceRanges->at(static_cast<int>(oldVal))
                 .name;
   const std::string compName =
       (-1 == newVal)
           ? ""
           : paramDescr.source.midi->sourceRanges->at(static_cast<int>(newVal))
                 .name;

   m_paramStorage.forEachParameter(
       [this, voiceIdx, &compNamePrev, &compName](
           int paramIdx, ParameterStorageElement& element) {
          const auto& descr =
              m_rSoundSection.parameterDescription(voiceIdx, paramIdx);
          if (descr.component && *descr.component == compNamePrev)
          {
             element.enable(false);
          }
          if (descr.component && *descr.component == compName)
          {
             element.enable(true);
          }
       },
       voiceIdx);
   if (m_midiInMsgHandler)
   {
      m_midiInMsgHandler->changeMapping(voiceIdx, compNamePrev, compName);
   }
}

SoundHandler::~SoundHandler() =
    default;   // makes forward decl in unique_ptr happy

SoundHandler::SoundHandler(SoundHandler&& other) noexcept = default;

void SoundHandler::initMidiInHandler(std::shared_ptr<MidiInput> pMidiIn,
                                     uint8_t midiVoiceOffset) noexcept
{
   assert(!m_midiInMsgHandler);
   m_midiInMsgHandler = std::make_unique<MidiInMsgHandlerT>(
       pMidiIn, m_rSoundSection, midiVoiceOffset,
       [this](int voiceIdx, int parameterId, float value) {
          // spdlog::info( "Received parameter values {} {} {}: ", voiceIdx,
          // parameterId, value);
          m_paramStorage.setSoundParameterActualValue(voiceIdx, parameterId,
                                                      value);
       });
   m_presetHandler.initMidiInHandler(m_midiInMsgHandler.get());
   if (m_midiOutHandler)
   {
      m_presetHandler.resetToInitialActualSoundPresetsIfSet();
      doParameterDumpRequest();
   }
}

void SoundHandler::initMidiOutHandler(std::shared_ptr<MidiOutput> pMidiOut,
                                      uint8_t midiVoiceOffset) noexcept
{
   assert(!m_midiOutHandler);
   m_midiOutHandler = std::make_unique<MidiOutMsgHandlerT>(
       pMidiOut, m_rSoundSection, midiVoiceOffset);
   m_presetHandler.initMidiOutHandler(m_midiOutHandler.get());
   if (m_midiInMsgHandler)
   {
      m_presetHandler.resetToInitialActualSoundPresetsIfSet();
      doParameterDumpRequest();
   }
}

void SoundHandler::initEvdevHandler()
{
   // TODO
}

void SoundHandler::setAmp(int voiceIdx, float amp) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "setAmp() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   m_rSoundSection.forEachParameterDescr(engineIdx,
       [this, voiceIdx, amp](int paramIdx, const auto& paramDescr) {
          if (paramDescr.role == description::sound::ParameterRole::Volume)
          {
            setParameterValue(voiceIdx, paramIdx, ParameterAttr::Commanded,
                               amp);
            return;
          }
       });
}

void SoundHandler::noteOn(int voiceIdx, int note, float velocity) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "noteOn() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   if (voiceIdx == base::musicDevice::description::sound::GlobalSectionId)
   {
      spdlog::error("noteOn() called for global section for device '{}'",
                    m_deviceName);
      return;
   }
   m_arpeggiators[voiceIdx].noteOn(note, velocity);
}

void SoundHandler::noteOff(int voiceIdx, int note, float velocity) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "noteOff() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   if (voiceIdx == base::musicDevice::description::sound::GlobalSectionId)
   {
      spdlog::error("noteOff() called for global section for device '{}'",
                    m_deviceName);
      return;
   }
   m_arpeggiators[voiceIdx].noteOff(note, velocity);
}

void SoundHandler::pitchBend(int voiceIdx, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "pitchBend() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   m_midiOutHandler->pitchBend(voiceIdx, value);
}

void SoundHandler::afterTouchPoly(int voiceIdx, int note, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "afterTouchPoly() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return;
   }
   m_midiOutHandler->afterTouchPoly(voiceIdx, note, value);
}

void SoundHandler::afterTouch(int voiceIdx, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "afterTouch() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   m_midiOutHandler->afterTouch(voiceIdx, value);
}

void SoundHandler::setParameterValue(int voiceIdx, int parameterId,
                                     ParameterAttr parameterAttr, float value,
                                     bool callCalcActVal) noexcept
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         setCommandedValue(voiceIdx, parameterId, value, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoFrequency):
      {
         setLFOFrequency(voiceIdx, parameterId, value, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoAmplitude):
      {
         setLFOAmplitude(voiceIdx, parameterId, value, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoWaveform):
      {
         setLFOWaveform(voiceIdx, parameterId, static_cast<lfo::Waveform>(value),
                        callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         setLFOMultiplierExp(voiceIdx, parameterId, static_cast<int>(value),
                             callCalcActVal);
         break;
      }
   }
}

void SoundHandler::setRelativeParameterValue(
    int voiceIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float relValue,
    bool callCalcActVal) const
{
   /*Not yet implemented, and I think its not so important*/
}

void SoundHandler::setCommandedValue(int voiceIdx, int parameterId, float value,
                                     bool callCalcActVal) noexcept
{
   if (m_midiOutHandler)
   {
      m_paramStorage.setSoundParameterValue(voiceIdx, parameterId, value, IncrementMode::Limit);
      if (callCalcActVal)
      {
         m_paramStorage.calcActualVal(voiceIdx, parameterId);
      }
   }
}

std::optional<float> SoundHandler::getParameterValue(
    int voiceIdx, int parameterId, ParameterAttr parameterAttr) const noexcept
{
   return m_paramStorage.getCommandedValue(voiceIdx, parameterId, parameterAttr);
}

float SoundHandler::fromNormalizedValue(int voiceIdx, int parameterId,
                                        ParameterAttr parameterAttr,
                                        float percentageValue) const noexcept
{
   const auto valueRangeEnd =
       getParameterRangeEnd(voiceIdx, parameterId, parameterAttr);
   return R_SWITCH(valueRangeEnd) FCASE(ListRangeEnd, range)->float
   {
      return float(int(range.get() * percentageValue));
   }
   , FCASE(FloatingPointRangeEnd, range)->float
   {
      return range.get() * percentageValue;
   }
   R_END_SWITCH
}

const base::musicDevice::description::sound::Parameter&
SoundHandler::parameterDescription(int voiceIdx, int parameterIdx) const
{
   return m_rSoundSection.parameterDescription(voiceIdx, parameterIdx);
}

ValueRangeEnd SoundHandler::getParameterRangeEnd(
    int voiceIdx, int parameterIdx, ParameterAttr parameterAttr) const
{
   return getParamRangeEnd(voiceIdx, parameterIdx, parameterAttr,
                           m_rSoundSection);
}

void SoundHandler::incrementParameterValue(int voiceIdx, int parameterId,
                                           ParameterAttr parameterAttr,
                                           float increment,
                                           IncrementMode incrementMode,
                                           bool callCalcActVal) noexcept
{
   switch (parameterAttr)
   {
      case (ParameterAttr::Commanded):
      {
         incCommandedValue(voiceIdx, parameterId, increment, incrementMode,
                           callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoFrequency):
      {
         incLFOFrequency(voiceIdx, parameterId, increment, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoAmplitude):
      {
         incLFOAmplitude(voiceIdx, parameterId, increment, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoWaveform):
      {
         incLFOWaveform(voiceIdx, parameterId, static_cast<int>(increment),
                        incrementMode, callCalcActVal);
         break;
      }
      case (ParameterAttr::LfoMultiplierExp):
      {
         incLFOMultiplierExp(voiceIdx, parameterId, static_cast<int>(increment),
                             incrementMode, callCalcActVal);
         break;
      }
   }
}

void SoundHandler::incrementParameterValueEventBound(
    int voiceIdx, int parameterId, ParameterAttr parameterAttr, float increment,
    IncrementMode incrementMode, bool callCalcActVal) noexcept
{
   // Not shure if I want to implement this
}

void SoundHandler::incCommandedValue(int voiceIdx, int parameterId,
                                     float increment,
                                     IncrementMode incrementMode,
                                     bool callCalcActVal) noexcept
{
   if (m_midiOutHandler)
   {
      m_paramStorage.incSoundParameterValue(voiceIdx, parameterId, increment,
                                            incrementMode);
      if (callCalcActVal)
      {
         m_paramStorage.calcActualVal(voiceIdx, parameterId);
      }
   }
}

void SoundHandler::updateActualSoundStorageValues() noexcept
{
   if (m_midiOutHandler)
   {
      m_paramStorage.updateActualValuesIfLfoActive();
      for (auto& arp : m_arpeggiators) { arp.update(); }
   }
}

void SoundHandler::uiShowsInterestInParameter(int voiceIdx,
                                              int parameterId) noexcept
{
   m_paramStorage.uiShowsInterestInParameter(voiceIdx, parameterId);
}

void SoundHandler::uiLoosesInterestInParameter(int voiceIdx,
                                               int parameterId) noexcept
{
   m_paramStorage.uiLoosesInterestInParameter(voiceIdx, parameterId);
}

uint8_t SoundHandler::getMidiVoiceOffset() const noexcept
{
   if (!m_midiOutHandler)
      return 0;
   return m_midiOutHandler->getMidiChannelOffset();
}

void SoundHandler::blankVoiceParameter(int voiceIdx, int paramIdx) noexcept
{
   m_paramStorage.resetToInitialValue(voiceIdx, paramIdx);
}

void SoundHandler::blankVoiceParameters(int voiceIdx) noexcept
{
   m_paramStorage.resetToInitialValues(voiceIdx);
   m_paramStorage.calcActualValuesForVoice(voiceIdx);
}

void SoundHandler::blankAllVoiceParameters() noexcept
{
   m_paramStorage.resetToInitialValues();
   m_paramStorage.calcAllActualValues();
}

void SoundHandler::setLFOWaveform(int voiceIdx, int paramIdx,
                                  lfo::Waveform waveform,
                                  bool callCalcActVal) noexcept
{
   m_paramStorage.setWaveform(voiceIdx, paramIdx, waveform);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::setLFOAmplitude(int voiceIdx, int paramIdx, float amplitude,
                                   bool callCalcActVal) noexcept
{
   m_paramStorage.setAmplitude(voiceIdx, paramIdx, amplitude);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::setLFOFrequency(int voiceIdx, int paramIdx, float frequency,
                                   bool callCalcActVal) noexcept
{
   m_paramStorage.setFrequency(voiceIdx, paramIdx, frequency);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::setLFOMultiplierExp(int voiceIdx, int paramIdx,
                                       int multiplExp,
                                       bool callCalcActVal) noexcept
{
   m_paramStorage.setMultiplierExp(voiceIdx, paramIdx, multiplExp);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::incLFOWaveform(int voiceIdx, int paramIdx, int increment,
                                  IncrementMode incrementMode,
                                  bool callCalcActVal) noexcept
{
   int idx = static_cast<int>(m_paramStorage.waveform(voiceIdx, paramIdx)) +
             increment;
   static constexpr int lastIdx = static_cast<int>(lfo::Waveform::Random) + 1;
   if (incrementMode == IncrementMode::RoundRobin)
   {
      idx = idx % lastIdx;
   }
   if (idx >= static_cast<int>(lfo::Waveform::Sine) && idx < lastIdx)
   {
      m_paramStorage.setWaveform(voiceIdx, paramIdx,
                                 static_cast<lfo::Waveform>(idx));
      if (callCalcActVal)
      {
         m_paramStorage.calcActualVal(voiceIdx, paramIdx);
      }
   }
}

void SoundHandler::incLFOAmplitude(int voiceIdx, int paramIdx, float increment,
                                   bool callCalcActVal) noexcept
{
   m_paramStorage.setAmplitude(
       voiceIdx, paramIdx,
       m_paramStorage.amplitude(voiceIdx, paramIdx) + increment);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::incLFOFrequency(int voiceIdx, int paramIdx, float increment,
                                   bool callCalcActVal) noexcept
{
   m_paramStorage.setFrequency(
       voiceIdx, paramIdx,
       m_paramStorage.frequency(voiceIdx, paramIdx) + increment);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

void SoundHandler::incLFOMultiplierExp(int voiceIdx, int paramIdx,
                                       int increment,
                                       IncrementMode incrementMode,
                                       bool callCalcActVal) noexcept
{
   int newExponent =
       m_paramStorage.multiplierExp(voiceIdx, paramIdx) + increment;
   if (incrementMode == IncrementMode::RoundRobin)
   {
      newExponent = newExponent % (lfo::MAX_MULTIPLIER_EXP + 1);
   }
   m_paramStorage.setMultiplierExp(voiceIdx, paramIdx, newExponent);
   if (callCalcActVal)
   {
      m_paramStorage.calcActualVal(voiceIdx, paramIdx);
   }
}

std::shared_ptr<preset::DevicePresets> SoundHandler::presets() const noexcept
{
   return m_presetHandler.getSoundPresets();
}

void SoundHandler::doParameterDumpRequest() noexcept
{
   m_midiOutHandler->sendParameterDumpRequest();
}

const preset::PresetHandler& SoundHandler::presetHandler() const noexcept
{
   return m_presetHandler;
}

preset::PresetHandler& SoundHandler::presetHandler() noexcept
{
   return m_presetHandler;
}

std::vector<base::arp::Arpeggiator>& SoundHandler::arpeggiators() noexcept
{
   return m_arpeggiators;
}

void SoundHandler::applyModifier(int voiceIdx, int paramIdx,
                                 ParameterAttr parameterAttr, float destValue,
                                 float intensity) noexcept
{
   m_paramStorage.applyModifier(voiceIdx, paramIdx, parameterAttr, destValue,
                                intensity);
}

void SoundHandler::resetModifier(int voiceIdx, int paramIdx,
                                 ParameterAttr parameterAttr)
{
   m_paramStorage.resetModifier(voiceIdx, paramIdx, parameterAttr);
}

void SoundHandler::calcActualVal(int voiceIdx, int paramIdx)
{
   m_paramStorage.calcActualVal(voiceIdx, paramIdx);
}

bool SoundHandler::checkValidity(int voiceIdx, int parameterIdx) const noexcept
{
   return (-1 <= voiceIdx && voiceIdx < int(m_rSoundSection.voices.size())) &&
          (-1 <= parameterIdx &&
           parameterIdx <
               int(m_rSoundSection.engineBase(voiceIdx)->parameters.size()));
}

void SoundHandler::triggerUICallbacks() noexcept
{
   forEachParameter([this](int voiceIdx, int paramIdx,
                           sound::ParameterStorageElement& element) {
      const auto changedValues = element.uiAsksForChangedValues();
      if (changedValues)
      {
         emitSoundDevParamChanged(voiceIdx, paramIdx, changedValues->first,
                                  changedValues->second);
         element.lfo().uiAsksForChanges(
             [=, this](float value) {
                emitLFOAmplitudeChanged(voiceIdx, paramIdx, value);
             },
             [=, this](float value) {
                emitLFOFrequencyChanged(voiceIdx, paramIdx, value);
             },
             [=, this](lfo::Waveform value) {
                emitLFOWaveformChanged(voiceIdx, paramIdx, value);
             },
             [=, this](uint32_t value) {
                emitLFOMultiplierExpChanged(voiceIdx, paramIdx, value);
             });
      }
   });
}
