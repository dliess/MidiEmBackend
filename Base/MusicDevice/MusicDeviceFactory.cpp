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
#include <spdlog/spdlog.h>
#include <memory>

#define IGNORED_DEVICES "RtMidi", "Ableton Push 2", "Midi Through"

using namespace base::musicDevice;
using namespace base::musicDevice::factory;

Factory::Factory(Holder& rHolder, const std::string& resourceRootDir) :
    m_rHolder(rHolder),
    m_dataHolder(resourceRootDir),
    m_loader(resourceRootDir),
    m_musicDeviceInserter(rHolder, resourceRootDir)
{
#ifndef __INSERT_DUMMY_MIDI_DEVICES__
   midi::PortNotifiers::instance().inputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          spdlog::info( "--> input added: {}", devOnUsbPort.getDeviceName());
          auto pMidiIn =
              createMidi<MusicDevice::MidiInput, midi::UsbMidiIn>(index);
          if (!pMidiIn)
          {
             spdlog::error( "Could not create pMidiIn");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          switch(resType)
          {
              case Loader::ResultType::MusicDevice:
              {
                  if(devOnUsbPort.getMidiPort() > 0) return;
                  break;
              }
              case Loader::ResultType::NotFound:
              {
                  break;
              }
              case Loader::ResultType::MarkedUnused:
              {
                  return;
              }
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          auto pDescr = m_dataHolder.getDescription(deviceId.deviceName);
          if (pDescr->soundSection && pDescr->soundSection->canDumpPresets())
          {
             auto it = m_soundPresetFetchers.find(deviceId);
             if (it != m_soundPresetFetchers.end())
             {
                it->second.addMidiIn(std::move(pMidiIn));
                if (it->second.hasMidiInAndOut())
                {
                   it->second.fetchPresets();
                }
                fillActionQueueForMidiIn(deviceId, it->second.hijackMidiIn());
                fillActionQueueForMidiOut(deviceId, it->second.hijackMidiOut());
                m_soundPresetFetchers.erase(deviceId);
             }
             else
             {
                auto [it, inserted] = m_soundPresetFetchers.emplace(
                    std::piecewise_construct, std::forward_as_tuple(deviceId),
                    std::forward_as_tuple(std::move(pDescr)));
                it->second.addMidiIn(std::move(pMidiIn));
                it->second.onPresetReceived(
                    [this, deviceId](int engineIdx,
                                     const std::string& presetName,
                                     sound::preset::Preset&& preset) {
                       m_dataHolder.getDevicePresets(deviceId.deviceName)
                           ->savePreset(engineIdx, presetName,
                                        std::move(preset));
                       m_dataHolder.soundDevicesPresetChanged(sound::preset::Id(
                           {deviceId.deviceName, engineIdx, presetName}));
                    });
             }
          }
          else
          {
             fillActionQueueForMidiIn(deviceId, std::move(pMidiIn));
          }
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().outputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          spdlog::info( "--> output added: {}", devOnUsbPort.getDeviceName());
          auto pMidiOut =
              createMidi<MusicDevice::MidiOutput, midi::UsbMidiOut>(index);
          if (!pMidiOut)
          {
             spdlog::error( "Could not create pMidiOut");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          switch(resType)
          {
              case Loader::ResultType::MusicDevice:
              {
                  if(devOnUsbPort.getMidiPort() > 0) return;
                  break;
              }
              case Loader::ResultType::NotFound:
              {
                  break;
              }
              case Loader::ResultType::MarkedUnused:
              {
                  return;
              }
          }
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          auto pDescr = m_dataHolder.getDescription(deviceId.deviceName);
          if (pDescr->soundSection && pDescr->soundSection->canDumpPresets())
          {
             auto it = m_soundPresetFetchers.find(deviceId);
             if (it != m_soundPresetFetchers.end())
             {
                it->second.addMidiOut(std::move(pMidiOut));
                if (it->second.hasMidiInAndOut())
                {
                   it->second.fetchPresets();
                }
                fillActionQueueForMidiIn(deviceId, it->second.hijackMidiIn());
                fillActionQueueForMidiOut(deviceId, it->second.hijackMidiOut());
                m_soundPresetFetchers.erase(deviceId);
             }
             else
             {
                m_soundPresetFetchers.emplace(
                    std::piecewise_construct, std::forward_as_tuple(deviceId),
                    std::forward_as_tuple(std::move(pDescr)));
                it->second.addMidiOut(std::move(pMidiOut));
                it->second.onPresetReceived(
                    [this, deviceId](int engineIdx,
                                     const std::string& presetName,
                                     sound::preset::Preset&& preset) {
                       m_dataHolder.getDevicePresets(deviceId.deviceName)
                           ->savePreset(engineIdx, presetName,
                                        std::move(preset));
                       m_dataHolder.soundDevicesPresetChanged(sound::preset::Id(
                           {deviceId.deviceName, engineIdx, presetName}));
                    });
             }
          }
          else
          {
             fillActionQueueForMidiOut(deviceId, std::move(pMidiOut));
          }
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().inputs.registerRemovedPortCb(
       [this](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          spdlog::info( "<-- input removed: {}", devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          m_soundPresetFetchers.erase(deviceId);
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
          spdlog::info( "<-- output removed: {}", devOnUsbPort.getDeviceName());
          const auto [resType, deviceName] =
              m_loader.getMatchType(devOnUsbPort.getDeviceName());
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          m_soundPresetFetchers.erase(deviceId);
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
                    m_dataHolder.getDescription(lastDeviceId.deviceName),
                    m_dataHolder.getDevicePresets(lastDeviceId.deviceName),
                    m_dataHolder.getActualDevicePresetNames(lastDeviceId),
                    midiVoiceOffset);
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

Factory::MusicDeviceInserter::MusicDeviceInserter(
    Holder& rHolder, const std::string& resourceRootDir) noexcept :
    m_rHolder(rHolder), m_resourceRootDir(resourceRootDir)
{
}

std::shared_ptr<MusicDevice> Factory::MusicDeviceInserter::findOrCreateDevice(
    const MusicDeviceId& deviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut) noexcept
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
         pMusicDevice = createMusicDevice(
             deviceId, getMidiDevIdFrom(pMidiIn, pMidiOut), std::move(pDescr),
             std::move(pPresets), std::move(pActualPresetNames));
         // NOTE: Important to insert it first to trigger cb-signals in right
         // order
         m_rHolder.musicDevices.insert(
             std::make_pair(pMusicDevice->id(), pMusicDevice));
         if (pMidiIn)
         {
            pMusicDevice->initMidiIn(std::move(pMidiIn));
         }
         if (pMidiOut)
         {
            pMusicDevice->initMidiOut(std::move(pMidiOut));
         }
      }
      catch (std::exception& e)
      {
         spdlog::error( "Failed to add midi input medium'{}' {}",
               deviceId.toStr(), e.what());
         return nullptr;
      }
   }
   return pMusicDevice;
}

std::shared_ptr<MusicDevice> Factory::MusicDeviceInserter::createMusicDevice(
    const MusicDeviceId& deviceId, const MusicDeviceId& midiDeviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   spdlog::info( "Created Music Device {}", deviceId.toStr());
   auto pMusicDevice = std::make_shared<MusicDevice>(
       deviceId, midiDeviceId, m_resourceRootDir, std::move(pDescr),
       std::move(pPresets), std::move(pActualPresetNames));
   return std::move(pMusicDevice);
}

void Factory::fillActionQueueForMidiIn(
    const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn)
{
   util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
       .push(HandleMidiInInsert(), deviceId, pMidiIn,
             m_dataHolder.getDescription(deviceId.deviceName),
             m_dataHolder.getDevicePresets(deviceId.deviceName),
             m_dataHolder.getActualDevicePresetNames(deviceId));
   m_loader.forFirstDeviceInChain(
       deviceId, [this, pMidiIn](const MusicDeviceId& nextDeviceId,
                                 uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiInInsertChained(), nextDeviceId, pMidiIn,
                    m_dataHolder.getDescription(nextDeviceId.deviceName),
                    m_dataHolder.getDevicePresets(nextDeviceId.deviceName),
                    m_dataHolder.getActualDevicePresetNames(nextDeviceId));
       });
}

void Factory::fillActionQueueForMidiOut(
    const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut)
{
   util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
       .push(HandleMidiOutInsert(), deviceId, pMidiOut,
             m_dataHolder.getDescription(deviceId.deviceName),
             m_dataHolder.getDevicePresets(deviceId.deviceName),
             m_dataHolder.getActualDevicePresetNames(deviceId));
   m_loader.forEachDeviceInChain(
       deviceId, [this, pMidiOut](const MusicDeviceId& nextDeviceId,
                                  uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiOutInsertChained(), nextDeviceId, pMidiOut,
                    m_dataHolder.getDescription(nextDeviceId.deviceName),
                    m_dataHolder.getDevicePresets(nextDeviceId.deviceName),
                    m_dataHolder.getActualDevicePresetNames(nextDeviceId));
       });
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsert, MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   auto pDevice =
       findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                          std::move(pActualPresetNames), pMidiIn, nullptr);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiIn(pMidiIn);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsert, MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   auto pDevice =
       findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                          std::move(pActualPresetNames), nullptr, pMidiOut);
   if (!pDevice)
   {
      return;
   }
   m_rHolder.midiHolder.addMidiOut(pMidiOut);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiInInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                      std::move(pActualPresetNames), std::move(pMidiIn),
                      nullptr);
}

void Factory::MusicDeviceInserter::action(
    HandleMidiOutInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                      std::move(pActualPresetNames), nullptr,
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

MusicDeviceId Factory::MusicDeviceInserter::getMidiDevIdFrom(
    const std::shared_ptr<MusicDevice::MidiInput>& pMidiIn,
    const std::shared_ptr<MusicDevice::MidiOutput>& pMidiOut) noexcept
{
   if (pMidiIn)
      return {pMidiIn->medium().getDeviceName(), pMidiIn->medium().getPortName()};
   if (pMidiOut)
      return {pMidiOut->medium().getDeviceName(), pMidiOut->medium().getPortName()};
   assert(false);
   return MusicDeviceId();
}

void Factory::MusicDeviceInserter::action(
    HandleDeviceInsertChained, MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames,
    uint8_t midiVoiceOffset)
{
   auto pMusicDevice = createMusicDevice(
       deviceId, getMidiDevIdFrom(pMidiIn, pMidiOut), std::move(pDescr),
       std::move(pPresets), std::move(pActualPresetNames));
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
        const auto attrs = it->second->getPresetAttributes(engineIdx,
parameterName); if(attrs)
        {
            for(auto& cb : updatedCbs) cb(musicDeviceName, engineIdx,
parameterName, attrs->first, attrs->second );
        }
        else
        {
            for(auto& cb : removedCbs) cb(musicDeviceName, engineIdx,
parameterName);
        }
        //it->second->save();
    }
}
*/

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
void Factory::addDummy(const std::string& usbDeviceName) noexcept
{
   midi::MidiMediumDummy dummy(usbDeviceName, midi::IMidiMedium::Type::USB);

   const auto [resType, deviceName] = m_loader.getMatchType(usbDeviceName);
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