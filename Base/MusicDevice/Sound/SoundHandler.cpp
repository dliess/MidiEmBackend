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
   std::shared_ptr<SoundPresets> soundPresets) noexcept :
   m_deviceName(std::move(deviceName)),
   m_rSoundSection(rSoundSection),
   m_presetHandler(rSoundSection, m_paramStorage, std::move(soundPresets))
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
         m_paramStorage.setSoundParameterActualValue(voiceId, parameterId, value);
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
   m_midiOutHandler->sendSoundParameter(voiceId, parameterId, value);
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

void SoundHandler::updateUI() noexcept
{
   m_paramStorage.updateUI();
}

void SoundHandler::registerParamChangeCbUI(ParamChangeCb cb)
{
   m_paramStorage.registerParamChangeCbUI(cb);
}

void SoundHandler::uiShowsInterestInParameter(int voiceId, int parameterId) noexcept
{
   m_paramStorage.uiShowsInterestInParameter(voiceId, parameterId);
}

void SoundHandler::uiLoosesInterestInParameter(int voiceId, int parameterId) noexcept
{
   m_paramStorage.uiLoosesInterestInParameter(voiceId, parameterId);
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
