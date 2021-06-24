#include "SoundHandler.h"

#include <loguru.hpp>

#include "Midi1Output.h"
#include "SoundMidiInMsgHandler.h"
#include "SoundMidiOutMsgHandler.h"
#include "SoundPresets.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound;

SoundHandler::SoundHandler(
   std::string deviceName, const description::sound::Section& rSoundSection,
   std::shared_ptr<SoundPresets> soundPresets,
   uint8_t midiVoiceOffset) noexcept :
   m_deviceName(std::move(deviceName)),
   m_rSoundSection(rSoundSection),
   m_presetHandler(rSoundSection, m_paramStorage, std::move(soundPresets)),
   m_midiVoiceOffset(midiVoiceOffset)
{
   m_paramStorage.resizeBy(rSoundSection);
}

SoundHandler::~SoundHandler() =
   default; // makes forward decl in unique_ptr happy


SoundHandler::SoundHandler(SoundHandler&& other) noexcept = default;

void SoundHandler::initMidiInHandler(
   std::shared_ptr<MidiInput> pMidiIn) noexcept
{
   assert(!m_midiInMsgHandler);
   m_midiInMsgHandler = std::make_unique<MidiInMsgHandlerT>(
      pMidiIn, m_rSoundSection, [this](int voiceId, int parameterId, float value) {
         //LOG_F(INFO, "Received parameter values {} {} {}: ", voiceId, parameterId, value);
         m_paramStorage.setSoundParameterActualValue(voiceId - m_midiVoiceOffset, parameterId, value);
      });
   if(m_midiOutHandler)
   {
      doParameterDumpRequest();
   }
}

void SoundHandler::initMidiOutHandler(
   std::shared_ptr<MidiOutput> pMidiOut) noexcept
{
   assert(!m_midiOutHandler);
   m_midiOutHandler =
      std::make_unique<MidiOutMsgHandlerT>(pMidiOut, m_rSoundSection);
   if(m_midiInMsgHandler)
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
   m_midiOutHandler->noteOn(voiceIndex + m_midiVoiceOffset, note, velocity);
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
   m_midiOutHandler->noteOff(voiceIndex + m_midiVoiceOffset, note, velocity);
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
   m_midiOutHandler->pitchBend(voiceIndex + m_midiVoiceOffset, value);
}

void SoundHandler::afterTouchPoly(int voiceIndex, int note, float value) noexcept
{
   if (!m_midiOutHandler)
   {
      LOG_F(
         ERROR,
         "afterTouchPoly() called but there is no m_midiOutHandler in device '{}'",
         m_deviceName);
      return;
   }
   m_midiOutHandler->afterTouchPoly(voiceIndex + m_midiVoiceOffset, note, value);
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
   m_midiOutHandler->afterTouch(voiceIndex + m_midiVoiceOffset, value);
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
   if(value < 0.0 || value >= 1.0) return;
   m_paramStorage.setSoundParameterValue(voiceId, parameterId, value);
   m_midiOutHandler->sendSoundParameter(voiceId + m_midiVoiceOffset, parameterId, value);
}

void SoundHandler::incrementParameterValue(int voiceId, int parameterId, float increment) noexcept
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
   const float actualValue = m_paramStorage.getCommandedValue(voiceId, parameterId);
   setParameterValue(voiceId, parameterId, actualValue + increment);
}

void SoundHandler::updateActualSoundStorageValues() noexcept
{
   if (m_midiOutHandler)
   {
      m_paramStorage.updateActualValues(
         [this](int voiceIdx, int paramIdx, float value) {
            m_midiOutHandler->sendSoundParameter(voiceIdx + m_midiVoiceOffset, paramIdx, value);
         });
   }
}

void SoundHandler::uiShowsInterestInParameter(int voiceId, int parameterId) noexcept
{
   m_paramStorage.uiShowsInterestInParameter(voiceId, parameterId);
}

void SoundHandler::uiLoosesInterestInParameter(int voiceId, int parameterId) noexcept
{
   m_paramStorage.uiLoosesInterestInParameter(voiceId, parameterId);
}

uint8_t SoundHandler::getMidiVoiceOffset() const noexcept
{
   return m_midiVoiceOffset;
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
      //m_paramStorage.markAllDirty();
   }
}
