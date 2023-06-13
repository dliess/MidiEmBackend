#include "SoundHandler.h"

#include <spdlog/spdlog.h>

#include "DevicePresets.h"
#include "Midi1Output.h"
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
   m_paramStorage.onActualPresetChanged(
       [this](int voiceIdx, const std::string& presetName) {
          emitActualPresetChanged(voiceIdx, presetName);
       });
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
       [this](int voiceId, int parameterId, float value) {
          //spdlog::info( "Received parameter values {} {} {}: ", voiceId,
          // parameterId, value);
          m_paramStorage.setSoundParameterActualValue(voiceId, parameterId,
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

void SoundHandler::afterTouchPoly(int voiceIdx, int note,
                                  float value) noexcept
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

void SoundHandler::setParameterValue(int voiceId, int parameterId, ParameterAttr parameterAttr,
                                     float value) noexcept
{
   switch(parameterAttr) {
      case(ParameterAttr::Commanded): { setCommandedValue(voiceId, parameterId, value); break; }
      case(ParameterAttr::LfoFrequency): { setLFOFrequency(voiceId, parameterId, value); break; }
      case(ParameterAttr::LfoAmplitude): { setLFOAmplitude(voiceId, parameterId, value); break; }
      case(ParameterAttr::LfoWaveform): { setLFOWaveform(voiceId, parameterId, static_cast<lfo::Waveform>(value)); break; }
      case(ParameterAttr::LfoMultiplierExp): { setLFOMultiplierExp(voiceId, parameterId, static_cast<int>(value)); break; }
   }
}

void SoundHandler::setCommandedValue(int voiceId, int parameterId,
                                     float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "setSoundParameterValue() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return;
   }
   m_paramStorage.setSoundParameterValue(voiceId, parameterId, value);
}

float SoundHandler::getParameterValue(
    int voiceId, int parameterId, ParameterAttr parameterAttr) const noexcept
{
   return m_paramStorage.getCommandedValue(voiceId, parameterId, parameterAttr);
}

float SoundHandler::normalizePercentageValue(
    int voiceId, int parameterId, ParameterAttr parameterAttr,
    float percentageValue) const noexcept
{
   const auto range = getParameterRange(voiceId, parameterId, parameterAttr);
   const auto& pd   = m_rSoundSection.parameterDescr(voiceId, parameterId);
   if (pd.type == description::sound::Parameter::Type::List)
   {
      return static_cast<int>(range * percentageValue);
   }
   return range * percentageValue;
}

const base::musicDevice::description::sound::Parameter* SoundHandler::parameterDescription(
    int voiceIdx, int parameterIdx) const
{
   // TODO
   return &m_rSoundSection.parameterDescr(voiceIdx, parameterIdx);
}

float SoundHandler::getParameterRange(int voiceId, int parameterId,
                                      ParameterAttr parameterAttr) const
{
   float maxVal = 1.0;
   switch (parameterAttr)
   {
      case ParameterAttr::Commanded:
      {
         const auto& pd = m_rSoundSection.parameterDescr(voiceId, parameterId);
         if (pd.type == description::sound::Parameter::Type::List)
         {
            maxVal = pd.source.midi->sourceRanges->size();
         }
         else
         {
            maxVal = 1.0;
         }
         break;
      }
      case ParameterAttr::LfoAmplitude:
      {
         maxVal = 1.0;
         break;
      }
      case ParameterAttr::LfoFrequency:
      {
         maxVal = 1.0;
         break;
      }
      case ParameterAttr::LfoWaveform:
      {
         maxVal = 4.0;
         break;
      }
      case ParameterAttr::LfoMultiplierExp:
      {
         maxVal = lfo::MAX_MULTIPLIER_EXP;
         break;
      }
   }
   return maxVal;
}

void SoundHandler::incrementParameterValue(int voiceId, int parameterId,
                                           ParameterAttr parameterAttr,
                                           float increment,
                                           bool roundRobin) noexcept
{
   switch(parameterAttr) {
      case(ParameterAttr::Commanded): { incCommandedValue(voiceId, parameterId, increment, roundRobin); break;}
      case(ParameterAttr::LfoFrequency): { incLFOFrequency(voiceId, parameterId, increment); break;}
      case(ParameterAttr::LfoAmplitude): { incLFOAmplitude(voiceId, parameterId, increment); break;}
      case(ParameterAttr::LfoWaveform): { incLFOWaveform(voiceId, parameterId, static_cast<int>(increment), roundRobin); break; }
      case(ParameterAttr::LfoMultiplierExp): { incLFOMultiplierExp(voiceId, parameterId, static_cast<int>(increment), roundRobin); break;}
   }
}

void SoundHandler::incCommandedValue(int voiceId, int parameterId,
                                           float increment,
                                           bool roundRobin) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "setSoundParameterValue() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return;
   }
   m_paramStorage.incSoundParameterValue(voiceId, parameterId, increment,
                                         roundRobin);
}

void SoundHandler::updateActualSoundStorageValues() noexcept
{
   if (m_midiOutHandler)
   {
      for (auto& arp : m_arpeggiators) { arp.update(); }
      m_paramStorage.updateActualValues(
          [this](int voiceIdx, int paramIdx, float value, float prevValue) {
             const auto& paramDescr =
                 m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
             if (paramDescr.role ==
                 description::sound::Parameter::Role::ComponentSelector)
             {
                assert(paramDescr.source.midi->sourceRanges);
                const std::string compNamePrev =
                    (-1 == prevValue) ? ""
                                      : paramDescr.source.midi->sourceRanges
                                            ->at(static_cast<int>(prevValue))
                                            .name;
                const std::string compName =
                    (-1 == value) ? ""
                                  : paramDescr.source.midi->sourceRanges
                                        ->at(static_cast<int>(value))
                                        .name;

                m_paramStorage.forEachParameter(
                    [this, voiceIdx, &compNamePrev, &compName](
                        int paramIdx, ParameterStorageElement& element) {
                       const auto& descr =
                           m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
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
                   m_midiInMsgHandler->changeMapping(voiceIdx, compNamePrev,
                                                     compName);
                }
             }
             m_midiOutHandler->sendSoundParameter(voiceIdx, paramIdx, value);
          });
   }
}

void SoundHandler::uiShowsInterestInParameter(int voiceId,
                                              int parameterId) noexcept
{
   m_paramStorage.uiShowsInterestInParameter(voiceId, parameterId);
}

void SoundHandler::uiLoosesInterestInParameter(int voiceId,
                                               int parameterId) noexcept
{
   m_paramStorage.uiLoosesInterestInParameter(voiceId, parameterId);
}

uint8_t SoundHandler::getMidiVoiceOffset() const noexcept
{
   if (!m_midiOutHandler)
      return 0;
   return m_midiOutHandler->getMidiChannelOffset();
}

void SoundHandler::blankVoiceParameter(int voiceId, int paramIdx) noexcept
{
   m_paramStorage.resetToInitialValue(voiceId, paramIdx);
}

void SoundHandler::blankVoiceParameters(int voiceId) noexcept
{
   m_paramStorage.resetToInitialValues(voiceId);
}

void SoundHandler::blankAllVoiceParameters() noexcept
{
   m_paramStorage.resetToInitialValues();
}

void SoundHandler::setLFOWaveform(int voiceId, int paramIdx,
                                  lfo::Waveform waveform) noexcept
{
   m_paramStorage.setWaveform(voiceId, paramIdx, waveform);
}

void SoundHandler::setLFOAmplitude(int voiceIdx, int paramIdx,
                                   float amplitude) noexcept
{
   m_paramStorage.setAmplitude(voiceIdx, paramIdx, amplitude);
}

void SoundHandler::setLFOFrequency(int voiceIdx, int paramIdx,
                                   float frequency) noexcept
{
   m_paramStorage.setFrequency(voiceIdx, paramIdx, frequency);
}

void SoundHandler::setLFOMultiplierExp(int voiceIdx, int paramIdx,
                                       int multiplExp) noexcept
{
   m_paramStorage.setMultiplierExp(voiceIdx, paramIdx, multiplExp);
}

void SoundHandler::incLFOWaveform(int voiceIdx, int paramIdx,
                                  int increment, bool roundRobin) noexcept
{
   int idx =
       static_cast<int>(m_paramStorage.waveform(voiceIdx, paramIdx)) +
       increment;
   static constexpr int lastIdx = static_cast<int>(lfo::Waveform::Random) + 1;
   if (roundRobin) 
   {
      idx = idx % lastIdx;
   }
   if (idx >= static_cast<int>(lfo::Waveform::Sine) &&
       idx < lastIdx)
   {
      m_paramStorage.setWaveform(voiceIdx, paramIdx,
                                 static_cast<lfo::Waveform>(idx));
   }
}

void SoundHandler::incLFOAmplitude(int voiceIdx, int paramIdx,
                                   float increment) noexcept
{
   m_paramStorage.setAmplitude(
       voiceIdx, paramIdx,
       m_paramStorage.amplitude(voiceIdx, paramIdx) + increment);
}

void SoundHandler::incLFOFrequency(int voiceIdx, int paramIdx,
                                   float increment) noexcept
{
   m_paramStorage.setFrequency(
       voiceIdx, paramIdx,
       m_paramStorage.frequency(voiceIdx, paramIdx) + increment);
}

void SoundHandler::incLFOMultiplierExp(int voiceIdx, int paramIdx,
                                       int increment, bool roundRobin) noexcept
{
   int newExponent = m_paramStorage.multiplierExp(voiceIdx, paramIdx) + increment;
   if(roundRobin) 
   {
      newExponent = newExponent % (lfo::MAX_MULTIPLIER_EXP + 1);
   }
   m_paramStorage.setMultiplierExp(
       voiceIdx, paramIdx, newExponent);
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
