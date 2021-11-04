#include "MusicDeviceFactory.h"

#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceId.h"
#include "DevicePresets.h"
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
    m_rHolder(rHolder),
    m_descriptionLoader(resourceRootDir),
    m_musicDeviceInserter(rHolder, resourceRootDir)
{
#ifndef __INSERT_DUMMY_MIDI_DEVICES__
   midi::PortNotifiers::instance().inputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "------------------> input added: {}",
                devOnUsbPort.getDeviceName());
          auto pMidiIn =
              createMidi<MusicDevice::MidiInput, midi::UsbMidiIn>(index);
          if (!pMidiIn)
          {
             LOG_F(ERROR, "Could not create pMidiIn");
             return;
          }
          const auto [resType, deviceName] =
              m_descriptionLoader.getMatchType(devOnUsbPort.getDeviceName());
          if (resType == description::Loader::ResultType::MarkedUnused)
          {
             return;
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiInInsert(), deviceId, pMidiIn,
                    getDescription(deviceId));
          m_descriptionLoader.forFirstDeviceInChain(
              deviceId, [this, pMidiIn](const MusicDeviceId& nextDeviceId,
                                        uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(HandleMidiInInsertChained(), nextDeviceId, pMidiIn,
                           getDescription(nextDeviceId));
              });
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().outputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "------------------> output added: {}",
                devOnUsbPort.getDeviceName());
          auto pMidiOut =
              createMidi<MusicDevice::MidiOutput, midi::UsbMidiOut>(index);
          if (!pMidiOut)
          {
             LOG_F(ERROR, "Could not create pMidiOut");
             return;
          }
          const auto [resType, deviceName] =
              m_descriptionLoader.getMatchType(devOnUsbPort.getDeviceName());
          if (resType == description::Loader::ResultType::MarkedUnused)
          {
             return;
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiOutInsert(), deviceId, pMidiOut,
                    getDescription(deviceId));
          m_descriptionLoader.forEachDeviceInChain(
              deviceId, [this, pMidiOut](const MusicDeviceId& nextDeviceId,
                                         uint8_t midiVoiceOffset) {
                 util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
                     .push(HandleMidiOutInsertChained(), nextDeviceId, pMidiOut,
                           getDescription(nextDeviceId));
              });
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().inputs.registerRemovedPortCb(
       [this](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          LOG_F(INFO, "------------------> input removed: {}",
                devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_descriptionLoader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), deviceId);

          m_descriptionLoader.forEachDeviceInChain(
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
          LOG_F(INFO, "------------------> output removed: {}",
                devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_descriptionLoader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), deviceId);

          m_descriptionLoader.forEachDeviceInChain(
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
   return m_descriptionLoader.getAllDevicesAsJson();
}

void Factory::loadMusicDeviceToChain(const MusicDeviceId& chainRoot,
                                     const MusicDeviceName& device,
                                     uint8_t midiVoiceOffset)
{
   m_descriptionLoader.appendDeviceToChain(chainRoot, device, midiVoiceOffset);

   m_descriptionLoader.forLastDeviceInChain(
       chainRoot, [this, &chainRoot](const MusicDeviceId& lastDeviceId,
                                     uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleDeviceInsertChained(), lastDeviceId,
                    m_rHolder.midiHolder.getMidiIn(chainRoot),
                    m_rHolder.midiHolder.getMidiOut(chainRoot),
                    getDescription(lastDeviceId), midiVoiceOffset);
       });
}

void Factory::removeLastMusicDeviceFromChain(const MusicDeviceId& chainRoot)
{
   m_descriptionLoader.forLastDeviceInChain(
       chainRoot,
       [this](const MusicDeviceId& lastDeviceId, uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(EraseFromDevices(), lastDeviceId);
       });
   m_descriptionLoader.removeDeviceFromEndOf(chainRoot);
}

std::shared_ptr<description::Description> Factory::getDescription(
    const MusicDeviceId& deviceId) noexcept
{
   std::shared_ptr<description::Description> pDescr;
   auto itDescr = m_descriptionCache.find(deviceId);
   if (itDescr != m_descriptionCache.end())
   {
      pDescr = itDescr->second;
   }
   else
   {
      pDescr = m_descriptionLoader.load(deviceId.deviceName);   // can throw
      pDescr->checkValidity();                                  // can throw
      pDescr->initCaches();
      m_descriptionCache[deviceId] = pDescr;
   }
   return std::move(pDescr);
}

Factory::MusicDeviceInserter::MusicDeviceInserter(
    Holder& rHolder, const std::string& resourceRootDir) noexcept :
    m_rHolder(rHolder), m_resourceRootDir(resourceRootDir)
{
}

std::shared_ptr<MusicDevice> Factory::MusicDeviceInserter::findOrCreateDevice(
    const MusicDeviceId& deviceId,
    std::shared_ptr<description::Description> pDescr,
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
         pMusicDevice = createMusicDevice(deviceId, std::move(pDescr));
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
    std::shared_ptr<description::Description> pDescr)
{
   std::shared_ptr<sound::preset::DevicePresets> pSoundPresets;

   const auto itSimilarDev = m_rHolder.musicDevices.findByDeviceId(
       {deviceId.deviceName, MusicDeviceId::ANY_PORT});
   if (itSimilarDev != m_rHolder.musicDevices.end())
   {
      const auto pSimilarMusicDevice = itSimilarDev->second;
      pDescr                         = pSimilarMusicDevice->description();
      if (pSimilarMusicDevice->soundHandler)
      {
         pSoundPresets = pSimilarMusicDevice->soundHandler->presets();
      }
   }
   else
   {
      if (pDescr->soundSection && pDescr->soundSection->hasParameters())
      {
         pSoundPresets = std::make_shared<sound::preset::DevicePresets>(
             pDescr->manufacturer, pDescr->productName);
      }
   }

   LOG_F(INFO, "Created Music Device {}", deviceId.toStr());
   auto pMusicDevice = std::make_shared<MusicDevice>(
       deviceId, m_resourceRootDir, std::move(pDescr),
       std::move(pSoundPresets));
   return std::move(pMusicDevice);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsert, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr)
{
   auto pDevice =
       findOrCreateDevice(deviceId, std::move(pDescr), pMidiIn, nullptr);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiIn(pMidiIn);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsert, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr)
{
   auto pDevice =
       findOrCreateDevice(deviceId, std::move(pDescr), nullptr, pMidiOut);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiOut(pMidiOut);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pMidiIn), nullptr);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<base::musicDevice::MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr)
{
   findOrCreateDevice(deviceId, std::move(pDescr), nullptr,
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
    std::shared_ptr<description::Description> pDescr, uint8_t midiVoiceOffset)
{
   auto pMusicDevice = createMusicDevice(deviceId, std::move(pDescr));
   pMusicDevice->initMidiIn(std::move(pMidiIn), midiVoiceOffset);
   pMusicDevice->initMidiOut(std::move(pMidiOut), midiVoiceOffset);
   m_rHolder.musicDevices.insert(
       std::make_pair(pMusicDevice->id(), pMusicDevice));
}

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
void Factory::addDummy(const std::string& usbDeviceName) noexcept
{
   midi::MidiMediumDummy dummy(usbDeviceName, midi::IMidiMedium::Type::USB);

   const auto [resType, deviceName] =
       m_descriptionLoader.getMatchType(usbDeviceName);
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