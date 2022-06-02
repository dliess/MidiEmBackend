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
   m_paramStorage.onLFOWaveformChanged(
       [this](int voiceIdx, int parameterIdx, lfo::Waveform waveform) {
          emitLFOWaveformChanged(voiceIdx, parameterIdx, waveform);
       });
   m_paramStorage.onLFOAmplitudeChanged(
       [this](int voiceIdx, int parameterIdx, float amplitude) {
          emitLFOAmplitudeChanged(voiceIdx, parameterIdx, amplitude);
       });
   m_paramStorage.onLFOFrequencyChanged(
       [this](int voiceIdx, int parameterIdx, float frequency) {
          emitLFOFrequencyChanged(voiceIdx, parameterIdx, frequency);
       });
   m_paramStorage.onLFOMultiplierExpChanged(
       [this](int voiceIdx, int parameterIdx, uint32_t multiplExp) {
          emitLFOMultiplierExpChanged(voiceIdx, parameterIdx, multiplExp);
       });
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
          // spdlog::info( "Received parameter values {} {} {}: ", voiceId,
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

void SoundHandler::noteOn(int voiceIndex, int note, float velocity) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "noteOn() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   if (voiceIndex == base::musicDevice::description::sound::GlobalSectionId)
   {
      spdlog::error("noteOn() called for global section for device '{}'",
                    m_deviceName);
      return;
   }
   m_arpeggiators[voiceIndex].noteOn(note, velocity);
}

void SoundHandler::noteOff(int voiceIndex, int note, float velocity) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "noteOff() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   if (voiceIndex == base::musicDevice::description::sound::GlobalSectionId)
   {
      spdlog::error("noteOff() called for global section for device '{}'",
                    m_deviceName);
      return;
   }
   m_arpeggiators[voiceIndex].noteOff(note, velocity);
}

void SoundHandler::pitchBend(int voiceIndex, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "pitchBend() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   m_midiOutHandler->pitchBend(voiceIndex, value);
}

void SoundHandler::afterTouchPoly(int voiceIndex, int note,
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
   m_midiOutHandler->afterTouchPoly(voiceIndex, note, value);
}

void SoundHandler::afterTouch(int voiceIndex, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "afterTouch() called but there is no m_midiOutHandler in device '{}'",
          m_deviceName);
      return;
   }
   m_midiOutHandler->afterTouch(voiceIndex, value);
}

void SoundHandler::setParameterValue(int voiceId, int parameterId,
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

float SoundHandler::getParameterValue(int voiceId,
                                      int parameterId) const noexcept
{
   if (!m_midiOutHandler)
   {
      spdlog::error(
          "setSoundParameterValue() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return -1;
   }
   return m_paramStorage.getCommandedValue(voiceId, parameterId);
}

void SoundHandler::incrementParameterValue(int voiceId, int parameterId,
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
                        int paramIdx, ParameterStorage::Element& element) {
                       const auto& descr =
                           m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
                       if (descr.component && *descr.component == compNamePrev)
                       {
                          element.enabled = false;
                       }
                       if (descr.component && *descr.component == compName)
                       {
                          element.enabled     = true;
                          element.actual      = -1;
                          element.dirtyFlagRt = true;
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
   m_paramStorage.forEachParameter(
       [parameterId, voiceId, this](int paramIdx,
                                    ParameterStorage::Element& element) {
          if (parameterId == ALL || parameterId == paramIdx)
          {
             emitLFOWaveformChanged(voiceId, paramIdx, element.lfo.waveform());
             emitLFOAmplitudeChanged(voiceId, paramIdx,
                                     element.lfo.amplitude());
             emitLFOFrequencyChanged(voiceId, paramIdx,
                                     element.lfo.frequency());
             emitLFOMultiplierExpChanged(voiceId, paramIdx,
                                         element.lfo.multiplierExp());
          }
       },
       voiceId);
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
   // TODO
   m_paramStorage.forEachParameter(
       [paramIdx, voiceId, this](int _paramIdx,
                                 ParameterStorage::Element& element) {
          if (paramIdx == ALL || paramIdx == _paramIdx)
          {
             emitLFOWaveformChanged(voiceId, paramIdx, element.lfo.waveform());
             emitLFOAmplitudeChanged(voiceId, paramIdx,
                                     element.lfo.amplitude());
             emitLFOFrequencyChanged(voiceId, paramIdx,
                                     element.lfo.frequency());
             emitLFOMultiplierExpChanged(voiceId, paramIdx,
                                         element.lfo.multiplierExp());
          }
       },
       voiceId);
}

void SoundHandler::blankVoiceParameters(int voiceId) noexcept
{
   m_paramStorage.resetToInitialValues(voiceId);
   // TODO
   m_paramStorage.forEachParameter(
       [voiceId, this](int paramIdx, ParameterStorage::Element& element) {
          emitLFOWaveformChanged(voiceId, paramIdx, element.lfo.waveform());
          emitLFOAmplitudeChanged(voiceId, paramIdx, element.lfo.amplitude());
          emitLFOFrequencyChanged(voiceId, paramIdx, element.lfo.frequency());
          emitLFOMultiplierExpChanged(voiceId, paramIdx,
                                      element.lfo.multiplierExp());
       },
       voiceId);
}

void SoundHandler::blankAllVoiceParameters() noexcept
{
   m_paramStorage.resetToInitialValues();
   // TODO
   m_paramStorage.forEachParameter(
       [this](int voiceId, int paramIdx, ParameterStorage::Element& element) {
          emitLFOWaveformChanged(voiceId, paramIdx, element.lfo.waveform());
          emitLFOAmplitudeChanged(voiceId, paramIdx, element.lfo.amplitude());
          emitLFOFrequencyChanged(voiceId, paramIdx, element.lfo.frequency());
          emitLFOMultiplierExpChanged(voiceId, paramIdx,
                                      element.lfo.multiplierExp());
       });
}

void SoundHandler::setLFOWaveform(int voiceId, int paramIdx,
                                  lfo::Waveform waveform) noexcept
{
   m_paramStorage.setWaveform(voiceId, paramIdx, waveform);
}

void SoundHandler::setLFOAmplitude(int voiceIndex, int paramIdx,
                                   float amplitude) noexcept
{
   m_paramStorage.setAmplitude(voiceIndex, paramIdx, amplitude);
}

void SoundHandler::setLFOFrequency(int voiceIndex, int paramIdx,
                                   float frequency) noexcept
{
   m_paramStorage.setFrequency(voiceIndex, paramIdx, frequency);
}

void SoundHandler::setLFOMultiplierExp(int voiceIndex, int paramIdx,
                                       int multiplExp) noexcept
{
   m_paramStorage.setMultiplierExp(voiceIndex, paramIdx, multiplExp);
}

void SoundHandler::incLFOWaveform(int voiceIndex, int paramIdx,
                                  int increment) noexcept
{
   const int idx =
       static_cast<int>(m_paramStorage.waveform(voiceIndex, paramIdx)) +
       increment;
   if (idx >= static_cast<int>(lfo::Waveform::Sine) &&
       idx <= static_cast<int>(lfo::Waveform::Random))
   {
      m_paramStorage.setWaveform(voiceIndex, paramIdx,
                                 static_cast<lfo::Waveform>(idx));
   }
}

void SoundHandler::incLFOAmplitude(int voiceIndex, int paramIdx,
                                   float increment) noexcept
{
   m_paramStorage.setAmplitude(
       voiceIndex, paramIdx,
       m_paramStorage.amplitude(voiceIndex, paramIdx) + increment);
}

void SoundHandler::incLFOFrequency(int voiceIndex, int paramIdx,
                                   float increment) noexcept
{
   m_paramStorage.setFrequency(
       voiceIndex, paramIdx,
       m_paramStorage.frequency(voiceIndex, paramIdx) + increment);
}

void SoundHandler::incLFOMultiplierExp(int voiceIndex, int paramIdx,
                                       int increment) noexcept
{
   m_paramStorage.setMultiplierExp(
       voiceIndex, paramIdx,
       m_paramStorage.multiplierExp(voiceIndex, paramIdx) + increment);
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

void SoundHandler::applyModifier(int voiceIndex, int paramIdx,
                                 ParameterPart parameterPart, float destValue,
                                 float intensity) noexcept
{
   m_paramStorage.applyModifier(voiceIndex, paramIdx, parameterPart, destValue,
                                intensity);
}