#include "MusicDevice.h"

#include <iostream>
#include <loguru.hpp>

#include "IMidiInMedium.h"
#include "IMidiOutMedium.h"
#include "JsonCast.h"
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MusicDeviceDescription.h"

using namespace base::musicDevice;

MusicDevice::MusicDevice(
   MusicDeviceId deviceId, const std::string& resourceRootDir,
   std::shared_ptr<description::Description> descr,
   std::shared_ptr<sound::SoundPresets> soundPresets) noexcept :
   m_deviceId(std::move(deviceId)),
   m_pDescr(std::move(descr)), m_pluginHandler(resourceRootDir)
{
   if (m_pDescr->soundSection)
   {
      soundHandler.emplace(deviceId.deviceName, *m_pDescr->soundSection,
                           std::move(soundPresets));
   }

   if (m_pDescr->controllerSection)
   {
      controllerHandler.emplace(deviceId.deviceName, m_pluginHandler,
                                *m_pDescr->controllerSection);
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
      LOG_F(ERROR, "Symbol not found in {}, detail: {}",
            m_pDescr->relDevicePath, e.what());
   }
}

MusicDevice::~MusicDevice() noexcept                   = default;
MusicDevice::MusicDevice(MusicDevice&& other) noexcept = default;

MusicDeviceId MusicDevice::deviceId() const noexcept { return m_deviceId; }

void MusicDevice::initMidiIn(std::shared_ptr<MidiInput> pMidiInput) noexcept
{
   assert(pMidiInput);
   if (soundHandler)
   {
      soundHandler->initMidiInHandler(pMidiInput);
   }
   if (controllerHandler)
   {
      controllerHandler->initMidiInHandler(pMidiInput);
   }
}

void MusicDevice::initMidiOut(std::shared_ptr<MidiOutput> pMidiOutput) noexcept
{
   assert(pMidiOutput);
   if (soundHandler)
   {
      soundHandler->initMidiOutHandler(pMidiOutput);
   }
   if (controllerHandler)
   {
      controllerHandler->initMidiOutHandler(pMidiOutput);
   }
}

std::shared_ptr<description::Description> MusicDevice::description()
   const noexcept
{
   return m_pDescr;
}
