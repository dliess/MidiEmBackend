#include "SoundHandler.h"

#include <loguru.hpp>

#include "Midi1Output.h"
#include "SoundMidiInMsgHandler.h"
#include "SoundMidiOutMsgHandler.h"
#include "DevicePresets.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound;

SoundHandler::SoundHandler(std::string deviceName,
                           const description::sound::Section& rSoundSection,
                           std::shared_ptr<preset::DevicePresets> soundPresets) noexcept
    :
    m_deviceName(std::move(deviceName)),
    m_rSoundSection(rSoundSection),
    m_paramStorage(rSoundSection),
    m_presetHandler(rSoundSection, m_paramStorage, std::move(soundPresets))
{
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
          // LOG_F(INFO, "Received parameter values {} {} {}: ", voiceId,
          // parameterId, value);
          m_paramStorage.setSoundParameterActualValue(voiceId, parameterId,
                                                      value);
       });
   if (m_midiOutHandler)
   {
      doParameterDumpRequest();
   }
}

void SoundHandler::initMidiOutHandler(std::shared_ptr<MidiOutput> pMidiOut,
                                      uint8_t midiVoiceOffset) noexcept
{
   assert(!m_midiOutHandler);
   m_midiOutHandler = std::make_unique<MidiOutMsgHandlerT>(
       pMidiOut, m_rSoundSection, midiVoiceOffset);
   if (m_midiInMsgHandler)
   {
      doParameterDumpRequest();
   }
   m_arpeggiators.resize(m_rSoundSection.voices.size());
   for(int voiceIdx = 0; voiceIdx < m_arpeggiators.size(); ++voiceIdx)
   {
      m_arpeggiators[voiceIdx].onNoteOn([this, voiceIdx](int note, float velocity){
         m_midiOutHandler->noteOn(voiceIdx, note, velocity);
      });
      m_arpeggiators[voiceIdx].onNoteOff([this, voiceIdx](int note, float velocity){
         m_midiOutHandler->noteOff(voiceIdx, note, velocity);
      });
      m_arpeggiators[voiceIdx].bypass(false);
      m_arpeggiators[voiceIdx].setRange(arp::RangeType::Octave, 1);
      m_arpeggiators[voiceIdx].setStepLength(240/2);
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
      LOG_F(ERROR,
            "noteOn() called but there is no m_midiOutHandler in device '{}'",
            m_deviceName);
      return;
   }
   if(voiceIndex == base::musicDevice::description::sound::GlobalSectionId)
   {
      LOG_F(ERROR,
            "noteOn() called for global section for device '{}'",
            m_deviceName);
      return;
   }
   m_arpeggiators[voiceIndex].noteOn(note, velocity);
}

void SoundHandler::noteOff(int voiceIndex, int note, float velocity) noexcept
{
   if (!m_midiOutHandler)
   {
      LOG_F(ERROR,
            "noteOff() called but there is no m_midiOutHandler in device '{}'",
            m_deviceName);
      return;
   }
   if(voiceIndex == base::musicDevice::description::sound::GlobalSectionId)
   {
      LOG_F(ERROR,
            "noteOff() called for global section for device '{}'",
            m_deviceName);
      return;
   }
   m_arpeggiators[voiceIndex].noteOff(note, velocity);
}

void SoundHandler::pitchBend(int voiceIndex, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      LOG_F(
          ERROR,
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
      LOG_F(ERROR,
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
      LOG_F(
          ERROR,
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
      LOG_F(
          ERROR,
          "setSoundParameterValue() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return;
   }
   m_paramStorage.setSoundParameterValue(voiceId, parameterId, value);
}

void SoundHandler::incrementParameterValue(int voiceId, int parameterId,
                                           float increment) noexcept
{
   if (!m_midiOutHandler)
   {
      LOG_F(
          ERROR,
          "setSoundParameterValue() called but there is no m_midiOutHandler in "
          "device '{}'",
          m_deviceName);
      return;
   }
   const float actualValue =
       m_paramStorage.getCommandedValue(voiceId, parameterId);
   setParameterValue(voiceId, parameterId, actualValue + increment);
}

void SoundHandler::updateActualSoundStorageValues() noexcept
{
   if (m_midiOutHandler)
   {
      for(auto& arp : m_arpeggiators)
      {
         arp.update();
      }
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
             for (auto& cb : m_lFOWaveformChangeCBs)
                cb(voiceId, paramIdx, element.lfo.waveform());
             for (auto& cb : m_lFOAmplitudeChangeCB)
                cb(voiceId, paramIdx, element.lfo.amplitude());
             for (auto& cb : m_lFOFrequencyChangeCB)
                cb(voiceId, paramIdx, element.lfo.frequency());
             for (auto& cb : m_lFOMultiplierExpChangeCB)
                cb(voiceId, paramIdx, element.lfo.multiplierExp());
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
             for (auto& cb : m_lFOWaveformChangeCBs)
                cb(voiceId, _paramIdx, element.lfo.waveform());
             for (auto& cb : m_lFOAmplitudeChangeCB)
                cb(voiceId, _paramIdx, element.lfo.amplitude());
             for (auto& cb : m_lFOFrequencyChangeCB)
                cb(voiceId, _paramIdx, element.lfo.frequency());
             for (auto& cb : m_lFOMultiplierExpChangeCB)
                cb(voiceId, _paramIdx, element.lfo.multiplierExp());
          }
       },
       voiceId);
}

void SoundHandler::blankVoiceParameters(int voiceId) noexcept
{
   m_paramStorage.resetToInitialValues(voiceId);
   // TODO
   m_paramStorage.forEachParameter(
       [voiceId, this](int _paramIdx, ParameterStorage::Element& element) {
          for (auto& cb : m_lFOWaveformChangeCBs)
             cb(voiceId, _paramIdx, element.lfo.waveform());
          for (auto& cb : m_lFOAmplitudeChangeCB)
             cb(voiceId, _paramIdx, element.lfo.amplitude());
          for (auto& cb : m_lFOFrequencyChangeCB)
             cb(voiceId, _paramIdx, element.lfo.frequency());
          for (auto& cb : m_lFOMultiplierExpChangeCB)
             cb(voiceId, _paramIdx, element.lfo.multiplierExp());
       },
       voiceId);
}

void SoundHandler::blankAllVoiceParameters() noexcept
{
   m_paramStorage.resetToInitialValues();
   // TODO
   m_paramStorage.forEachParameter(
       [this](int voiceId, int _paramIdx, ParameterStorage::Element& element) {
          for (auto& cb : m_lFOWaveformChangeCBs)
             cb(voiceId, _paramIdx, element.lfo.waveform());
          for (auto& cb : m_lFOAmplitudeChangeCB)
             cb(voiceId, _paramIdx, element.lfo.amplitude());
          for (auto& cb : m_lFOFrequencyChangeCB)
             cb(voiceId, _paramIdx, element.lfo.frequency());
          for (auto& cb : m_lFOMultiplierExpChangeCB)
             cb(voiceId, _paramIdx, element.lfo.multiplierExp());
       });
}

void SoundHandler::setLFOWaveform(int voiceId, int paramIdx,
                                  lfo::Waveform waveform) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceId, paramIdx);
   if (lfo.setWaveform(waveform))
   {
      for (auto& cb : m_lFOWaveformChangeCBs)
         cb(voiceId, paramIdx, lfo.waveform());
   }
}

void SoundHandler::setLFOAmplitude(int voiceIndex, int paramIdx,
                                   float amplitude) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setAmplitude(amplitude))
   {
      for (auto& cb : m_lFOAmplitudeChangeCB)
         cb(voiceIndex, paramIdx, lfo.amplitude());
   }
}

void SoundHandler::setLFOFrequency(int voiceIndex, int paramIdx,
                                   float frequency) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setFrequency(frequency))
   {
      for (auto& cb : m_lFOFrequencyChangeCB)
         cb(voiceIndex, paramIdx, lfo.frequency());
   }
}

void SoundHandler::setLFOMultiplierExp(int voiceIndex, int paramIdx,
                                   int multiplExp) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setMultiplierExp(multiplExp))
   {
      for (auto& cb : m_lFOMultiplierExpChangeCB)
         cb(voiceIndex, paramIdx, lfo.multiplierExp());
   }
}

void SoundHandler::incLFOWaveform(int voiceId, int paramIdx,
                                  int increment) noexcept
{
   auto& lfo     = m_paramStorage.lfoOf(voiceId, paramIdx);
   const int idx = static_cast<int>(lfo.waveform()) + increment;
   if (idx >= static_cast<int>(lfo::Waveform::Sine) &&
       idx <= static_cast<int>(lfo::Waveform::Random))
   {
      if (lfo.setWaveform(static_cast<lfo::Waveform>(idx)))
      {
         for (auto& cb : m_lFOWaveformChangeCBs)
            cb(voiceId, paramIdx, lfo.waveform());
      }
   }
}

void SoundHandler::incLFOAmplitude(int voiceIndex, int paramIdx,
                                   float increment) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setAmplitude(lfo.amplitude() + increment))
   {
      for (auto& cb : m_lFOAmplitudeChangeCB)
         cb(voiceIndex, paramIdx, lfo.amplitude());
   }
}

void SoundHandler::incLFOFrequency(int voiceIndex, int paramIdx,
                                   float increment) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setFrequency(lfo.frequency() + increment))
   {
      for (auto& cb : m_lFOFrequencyChangeCB)
         cb(voiceIndex, paramIdx, lfo.frequency());
   }
}

void SoundHandler::incLFOMultiplierExp(int voiceIndex, int paramIdx,
                                       int increment) noexcept
{
   auto& lfo = m_paramStorage.lfoOf(voiceIndex, paramIdx);
   if (lfo.setMultiplierExp(lfo.multiplierExp() + increment))
   {
      for (auto& cb : m_lFOMultiplierExpChangeCB)
         cb(voiceIndex, paramIdx, lfo.multiplierExp());
   }
}

void SoundHandler::registerLFOWaveformChangeCB(LFOWaveformChangeCB cb)
{
   m_lFOWaveformChangeCBs.push_back(cb);
}

void SoundHandler::registerLFOAmplitudeChangeCB(LFOAmplitudeChangeCB cb)
{
   m_lFOAmplitudeChangeCB.push_back(cb);
}

void SoundHandler::registerLFOFrequencyChangeCB(LFOFrequencyChangeCB cb)
{
   m_lFOFrequencyChangeCB.push_back(cb);
}

void SoundHandler::registerLFOMultiplierExpChangeCB(LFOMultiplierExpChangeCB cb)
{
   m_lFOMultiplierExpChangeCB.push_back(cb);
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

base::arp::Arpeggiator& SoundHandler::arpeggiator(int voiceIdx)
{
   return m_arpeggiators.at(voiceIdx);
}

