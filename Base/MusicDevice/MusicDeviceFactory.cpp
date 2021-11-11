#include "MusicDeviceFactory.h"

#include "DevicePresets.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceId.h"
#include "UsbMidiIn.h"
#include "UsbMidiOut.h"
#include "UsbMidiPortNotifier.h"
#include "itcActionSender.h"

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
#include "MidiMediumDummy.h"
#endif
#include <cassert>
#include <loguru.hpp>
#include <memory>

#define IGNORED_DEVICES "RtMidi", "Ableton Push 2", "Midi Through"

using namespace base::musicDevice;

Factory::Factory(Holder& rHolder, const std::string& resourceRootDir) :
    dataHolder(resourceRootDir),
    m_rHolder(rHolder),
    m_loader(resourceRootDir),
    m_musicDeviceInserter(rHolder, resourceRootDir)
{
#ifndef __INSERT_DUMMY_MIDI_DEVICES__
   midi::PortNotifiers::instance().inputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "--> input added: {}", devOnUsbPort.getDeviceName());
          auto pMidiIn =
              createMidi<MusicDevice::MidiInput, midi::UsbMidiIn>(index);
          if (!pMidiIn)
          {
             LOG_F(ERROR, "Could not create pMidiIn");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          if (resType == Loader::ResultType::MarkedUnused)
          {
             return;
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiInInsert(), deviceId, pMidiIn,
                    dataHolder.getDescription(deviceId.deviceName),
                    dataHolder.getDevicePresets(deviceId.deviceName));
          m_loader.forFirstDeviceInChain(
              deviceId, [this, pMidiIn](const MusicDeviceId& nextDeviceId,
                                        uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(HandleMidiInInsertChained(), nextDeviceId, pMidiIn,
                           dataHolder.getDescription(nextDeviceId.deviceName),
                           dataHolder.getDevicePresets(nextDeviceId.deviceName));
              });
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().outputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "--> output added: {}", devOnUsbPort.getDeviceName());
          auto pMidiOut =
              createMidi<MusicDevice::MidiOutput, midi::UsbMidiOut>(index);
          if (!pMidiOut)
          {
             LOG_F(ERROR, "Could not create pMidiOut");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          if (resType == Loader::ResultType::MarkedUnused)
          {
             return;
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiOutInsert(), deviceId, pMidiOut,
                    dataHolder.getDescription(deviceId.deviceName),
                    dataHolder.getDevicePresets(deviceId.deviceName));
          m_loader.forEachDeviceInChain(
              deviceId, [this, pMidiOut](const MusicDeviceId& nextDeviceId,
                                         uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(HandleMidiOutInsertChained(), nextDeviceId, pMidiOut,
                           dataHolder.getDescription(nextDeviceId.deviceName),
                           dataHolder.getDevicePresets(nextDeviceId.deviceName));
              });
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().inputs.registerRemovedPortCb(
       [this](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "<-- input removed: {}", devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), deviceId);

          m_loader.forEachDeviceInChain(
              deviceId, [this](const MusicDeviceId& nextDeviceId,
                               uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(EraseFromDevices(), nextDeviceId);
              });

          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromMidiInHolder(),
                    MidiHolder::Id(devOnUsbPort.getDeviceName(),
                                   devOnUsbPort.getUsbPortName()));
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().outputs.registerRemovedPortCb(
       [this](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "<-- output removed: {}", devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), deviceId);

          m_loader.forEachDeviceInChain(
              deviceId, [this](const MusicDeviceId& nextDeviceId,
                               uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(EraseFromDevices(), nextDeviceId);
              });
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromMidiOutHolder(),
                    MidiHolder::Id(devOnUsbPort.getDeviceName(),
                                   devOnUsbPort.getUsbPortName()));
       },
       {{}, {IGNORED_DEVICES}, false});
#else
   insertMusicDeviceDummies();
#endif
}

void Factory::invokeInserterQueueActions() { m_actionQueue.popCallAll(); }

std::string Factory::getAllDevicesAsJson() const
{
   return m_loader.getAllDevicesAsJson();
}

void Factory::loadMusicDeviceToChain(const MusicDeviceId& chainRoot,
                                     const MusicDeviceName& device,
                                     uint8_t midiVoiceOffset)
{
   m_loader.appendDeviceToChain(chainRoot, device, midiVoiceOffset);

   m_loader.forLastDeviceInChain(
       chainRoot, [this, &chainRoot](const MusicDeviceId& lastDeviceId,
                                     uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleDeviceInsertChained(), lastDeviceId,
                    m_rHolder.midiHolder.getMidiIn(chainRoot),
                    m_rHolder.midiHolder.getMidiOut(chainRoot),
                    dataHolder.getDescription(lastDeviceId.deviceName),
                    dataHolder.getDevicePresets(lastDeviceId.deviceName), midiVoiceOffset);
       });
}

void Factory::removeLastMusicDeviceFromChain(const MusicDeviceId& chainRoot)
{
   m_loader.forLastDeviceInChain(
       chainRoot,
       [this](const MusicDeviceId& lastDeviceId, uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), lastDeviceId);
       });
   m_loader.removeDeviceFromEndOf(chainRoot);
}

Factory::DataHolder::DataHolder(std::string configDir) noexcept:
    m_configDir(std::move(configDir.empty() ? "." : configDir))
{
}


std::shared_ptr<description::Description> Factory::DataHolder::getDescription(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<description::Description> pDescr;
   auto itDescr = descriptionCache.find(deviceName);
   if (itDescr != descriptionCache.end())
   {
      pDescr = itDescr->second;
   }
   else
   {
      pDescr = description::loadDescription(m_configDir, deviceName);   // can throw
      pDescr->checkValidity();                         // can throw
      pDescr->initCaches();
      descriptionCache[deviceName] = pDescr;
   }
   return std::move(pDescr);
}

std::shared_ptr<sound::preset::DevicePresets> Factory::DataHolder::getDevicePresets(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<sound::preset::DevicePresets> pPresets;
   auto it = presetCache.find(deviceName);
   if (it != presetCache.end())
   {
      pPresets = it->second;
   }
   else
   {   // TODO: only if it has sound section and sound parameters
       // if (pDescr->soundSection && pDescr->soundSection->hasParameters()) {
      // pPresets = m_loader.load(deviceName);   // can throw
      pPresets = std::make_shared<sound::preset::DevicePresets>(deviceName);
      presetCache[deviceName] = pPresets;
      //}
   }
   return std::move(pPresets);
}

Factory::MusicDeviceInserter::MusicDeviceInserter(
    Holder& rHolder, const std::string& resourceRootDir) noexcept :
    m_rHolder(rHolder), m_resourceRootDir(resourceRootDir)
{
}

std::shared_ptr<MusicDevice> Factory::MusicDeviceInserter::findOrCreateDevice(
    const MusicDeviceId& deviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput>
        pMidiOut) noexcept
{
   std::shared_ptr<MusicDevice> pMusicDevice;
   auto itCntrlDev = m_rHolder.musicDevices.findByDeviceId(deviceId);
   if (itCntrlDev != m_rHolder.musicDevices.end())
   {
      pMusicDevice = itCntrlDev->second;
      if (pMidiIn)
      {
         pMusicDevice->initMidiIn(std::move(pMidiIn));
      }
      if (pMidiOut)
      {
         pMusicDevice->initMidiOut(std::move(pMidiOut));
      }
   }
   else
   {
      try
      {
         pMusicDevice = createMusicDevice(deviceId, std::move(pDescr),
                                          std::move(pPresets));
         if (pMidiIn)
         {
            pMusicDevice->initMidiIn(std::move(pMidiIn));
         }
         if (pMidiOut)
         {
            pMusicDevice->initMidiOut(std::move(pMidiOut));
         }
         m_rHolder.musicDevices.insert(
             std::make_pair(pMusicDevice->id(), pMusicDevice));
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "Failed to add midi input medium'{}' {}",
               deviceId.toStr(), e.what());
         return nullptr;
      }
   }
   return pMusicDevice;
}

std::shared_ptr<MusicDevice> Factory::MusicDeviceInserter::createMusicDevice(
    const MusicDeviceId& deviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets)
{
   LOG_F(INFO, "Created Music Device {}", deviceId.toStr());
   auto pMusicDevice = std::make_shared<MusicDevice>(
       deviceId, m_resourceRootDir, std::move(pDescr), std::move(pPresets));
   return std::move(pMusicDevice);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsert, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets)
{
   auto pDevice = findOrCreateDevice(deviceId, std::move(pDescr),
                                     std::move(pPresets), pMidiIn, nullptr);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiIn(pMidiIn);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsert, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets)
{
   auto pDevice = findOrCreateDevice(deviceId, std::move(pDescr),
                                     std::move(pPresets), nullptr, pMidiOut);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiOut(pMidiOut);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                      std::move(pMidiIn), nullptr);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets), nullptr,
                      std::move(pMidiOut));
}

void Factory::MusicDeviceInserter::action(EraseFromDevices,
                                          const MusicDeviceId& deviceId)
{
   m_rHolder.musicDevices.eraseByDeviceId(deviceId);
}

void Factory::MusicDeviceInserter::action(EraseFromMidiInHolder,
                                          const MidiHolder::Id& holderId)
{
   m_rHolder.midiHolder.removeMidiIn(holderId);
}

void Factory::MusicDeviceInserter::action(EraseFromMidiOutHolder,
                                          const MidiHolder::Id& holderId)
{
   m_rHolder.midiHolder.removeMidiOut(holderId);
}

void Factory::MusicDeviceInserter::action(
    HandleDeviceInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    uint8_t midiVoiceOffset)
{
   auto pMusicDevice =
       createMusicDevice(deviceId, std::move(pDescr), std::move(pPresets));
   pMusicDevice->initMidiIn(std::move(pMidiIn), midiVoiceOffset);
   pMusicDevice->initMidiOut(std::move(pMidiOut), midiVoiceOffset);
   m_rHolder.musicDevices.insert(
       std::make_pair(pMusicDevice->id(), pMusicDevice));
}

/*
void Factory::DataHolder::onSoundDevicesPresetChanged(
    const MusicDeviceName& musicDeviceName, int engineIdx,
    const std::string& parameterName)
{
    auto it = presetCache.find(musicDeviceName);
    if(it != presetCache.end())
    {
        const auto attrs = it->second->getPresetAttributes(engineIdx, parameterName);
        if(attrs)
        {
            for(auto& cb : updatedCbs) cb(musicDeviceName, engineIdx, parameterName, attrs->first, attrs->second );
        }
        else
        {
            for(auto& cb : removedCbs) cb(musicDeviceName, engineIdx, parameterName);
        }
        //it->second->save();
    }
}
*/

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
void Factory::addDummy(const std::string& usbDeviceName) noexcept
{
   midi::MidiMediumDummy dummy(usbDeviceName, midi::IMidiMedium::Type::USB);

   const auto [resType, deviceName] =
       m_loader.getMatchType(usbDeviceName);
   if (resType == description::Loader::ResultType::MarkedUnused)
   {
      return;
   }
   const MusicDeviceId deviceId(deviceName, "DummyPort");
   auto pDevice = findOrCreateDevice(deviceId);
   if (!pDevice)
   {
      return;
   }
   auto pMidiIn = std::make_shared<MusicDevice::MidiInput>(
       std::move(dummy.hijackInMedium()));
   pDevice->initMidiIn(pMidiIn);
   handleDeviceChainIn(deviceId, pMidiIn);
   m_rHolder.midiHolder.addMidiIn(pMidiIn);
   auto pMidiOut = std::make_shared<MusicDevice::MidiOutput>(
       std::move(dummy.hijackOutMedium()));
   pDevice->initMidiOut(pMidiOut);
   handleDeviceChainOut(deviceId, pMidiOut);
   m_rHolder.midiHolder.addMidiOut(pMidiOut);
}

void Factory::insertMusicDeviceDummies()
{
   addDummy("Moog Minitaur:Moog Minitaur MIDI 1");
   addDummy("MIDIFACE 8x8:MIDIFACE 8x8 MIDI 1");
   addDummy("TOUCHE_SE:TOUCHE_SE MIDI 1");
   addDummy("Deluge:Deluge MIDI 1");
   addDummy("Arturia KeyStep 32:Arturia KeyStep 32 MIDI 1");
   addDummy("MODEL D:MODEL D MIDI 1");
   addDummy("Space Pedal:Space Pedal MIDI 1");
   addDummy("Launch Control XL:Launch Control XL MIDI 1");
   addDummy("Seaboard BLOCK:Seaboard BLOCK MIDI 1");
   addDummy("Buzzzy! polysynth:Buzzzy! polysynth MIDI 1");
   addDummy("MicroBrute:MicroBrute MIDI 1");
   addDummy("SUONOBUONO nABC:SUONOBUONO nABC MIDI 1");
   /*
   addDummy("__JUST_4_TEST__MAM/MB33");
   addDummy("__JUST_4_TEST__Korg/VolcaKeys");
   addDummy("__JUST_4_TEST__Korg/VolcaKick");
   addDummy("__JUST_4_TEST__Mitxela/Flash");
   addDummy("__JUST_4_TEST__Elektron/Octatrack_Mk1");
   addDummy("__JUST_4_TEST__Elektron/Analog4_Mk1");
   addDummy("__JUST_4_TEST__Elektron/AnalogRytm_Mk1");
   */
}

#endif