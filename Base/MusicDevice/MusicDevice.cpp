#include "MusicDevice.h"

#include <iostream>
#include <spdlog/spdlog.h>

#include "IMidiInMedium.h"
#include "IMidiOutMedium.h"
#include "JsonCast.h"
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MusicDeviceDescription.h"

using namespace base::musicDevice;

MusicDevice::MusicDevice(
    MusicDeviceId deviceId, MusicDeviceId midiMediumId,
    const std::string& resourceRootDir,
    std::shared_ptr<description::Description> descr,
    std::shared_ptr<sound::preset::DevicePresets> soundPresets,
    std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept :
    m_deviceId(std::move(deviceId)),
    m_mediumId(std::move(midiMediumId)),
    m_pDescr(std::move(descr)),
    m_pluginHandler(resourceRootDir)
{
   if (m_pDescr->soundSection)
   {
      soundHandler.emplace(deviceId.deviceName, *m_pDescr->soundSection,
                           std::move(soundPresets),
                           std::move(pActualPresetNames));
   }

   if (m_pDescr->controllerSection)
   {
      controllerHandler.emplace(deviceId.deviceName, m_pluginHandler,
                                *m_pDescr->controllerSection);
   }

   if (m_pDescr->reactsToTransportCommands &&
       *m_pDescr->reactsToTransportCommands)
   {
      sequencer.emplace();
   }

   try
   {
      m_pluginHandler.load(m_pDescr->relDevicePath);
   }
   catch (PluginHandler::PluginOpenError& e)
   {
      // not a problem;
   }
   catch (PluginHandler::SymbolError& e)
   {
      spdlog::error( "Symbol not found in {}, detail: {}",
            m_pDescr->relDevicePath, e.what());
   }
}

MusicDevice::~MusicDevice() noexcept                   = default;
MusicDevice::MusicDevice(MusicDevice&& other) noexcept = default;

MusicDeviceId MusicDevice::deviceId() const noexcept { return m_deviceId; }

void MusicDevice::initMidiIn(std::shared_ptr<MidiInput> pMidiInput,
                             uint8_t midiVoiceOffset) noexcept
{
   if(!pMidiInput)
   {
      spdlog::info("Device {} has no midi IN", m_deviceId.deviceName);
      return;
   }
   if (soundHandler)
   {
      soundHandler->initMidiInHandler(pMidiInput, midiVoiceOffset);
   }
   if (controllerHandler)
   {
      controllerHandler->initMidiInHandler(pMidiInput);
   }
}

void MusicDevice::initMidiOut(std::shared_ptr<MidiOutput> pMidiOutput,
                              uint8_t midiVoiceOffset) noexcept
{
   if(!pMidiOutput)
   {
      spdlog::info("Device {} has no midi OUT", m_deviceId.deviceName);
      return;
   }
   if (soundHandler)
   {
      soundHandler->initMidiOutHandler(pMidiOutput, midiVoiceOffset);
   }
   if (controllerHandler)
   {
      controllerHandler->initMidiOutHandler(pMidiOutput);
   }
   if (sequencer)
   {
      sequencer->initMidiOutHandler(pMidiOutput);
   }
}

const MusicDeviceId& MusicDevice::mediumId() const
{
   return m_mediumId;
}

std::shared_ptr<description::Description> MusicDevice::description()
    const noexcept
{
   return m_pDescr;
}
