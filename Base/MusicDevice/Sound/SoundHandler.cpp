#include "SoundHandler.h"

#include <loguru.hpp>

#include "Midi1Output.h"
#include "SoundMidiInMsgHandler.h"
#include "SoundMidiOutMsgHandler.h"
#include "SoundPresets.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound;

SoundHandler::SoundHandler(std::string deviceName,
                           const description::sound::Section& rSoundSection,
                           std::shared_ptr<SoundPresets> soundPresets) noexcept
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
   m_midiOutHandler->noteOn(voiceIndex, note, velocity);
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
   m_midiOutHandler->noteOff(voiceIndex, note, velocity);
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
   if (value < 0.0)
   {
      value = 0.0;
   }
   if (value >= 1.0)
   {
      value = 0.999999;
   }
   m_paramStorage.setSoundParameterValue(voiceId, parameterId, value);
   // m_midiOutHandler->sendSoundParameter(voiceId, parameterId, value);
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
      m_paramStorage.updateActualValues(
          [this](int voiceIdx, int paramIdx, float value) {
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
                                  LFO::Waveform waveform) noexcept
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

void SoundHandler::incLFOWaveform(int voiceId, int paramIdx,
                                  int increment) noexcept
{
   auto& lfo     = m_paramStorage.lfoOf(voiceId, paramIdx);
   const int idx = static_cast<int>(lfo.waveform()) + increment;
   if (idx >= static_cast<int>(LFO::Waveform::Sine) &&
       idx <= static_cast<int>(LFO::Waveform::Random))
   {
      if (lfo.setWaveform(static_cast<LFO::Waveform>(idx)))
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
                                       uint32_t increment) noexcept
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

std::shared_ptr<SoundPresets> SoundHandler::presets() const noexcept
{
   return m_presetHandler.getSoundPresets();
}

void SoundHandler::doParameterDumpRequest() noexcept
{
   if (!m_midiOutHandler->sendParameterDumpRequest())
   {
      // TODO: do something else here
      // m_paramStorage.markAllDirty();
   }
}
