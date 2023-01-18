#include "MusicDeviceFactory.h"

#include <spdlog/spdlog.h>

#include <cassert>
#include <memory>

#include "DevicePresets.h"
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "MusicDeviceId.h"
#include "UsbMidiIn.h"
#include "UsbMidiOut.h"
#include "UsbMidiPortNotifier.h"
#include "itcActionSender.h"

#define IGNORED_DEVICES "RtMidi", "Ableton Push 2"

using namespace base::musicDevice;
using namespace base::musicDevice::factory;

Factory::Factory(Holder& rHolder, const std::string& resourceRootDir) :
    m_rHolder(rHolder),
    m_dataHolder(resourceRootDir),
    m_loader(resourceRootDir),
    m_musicDeviceInserter(rHolder, m_dataHolder, resourceRootDir)
{
   midi::PortNotifiers::instance().inputs.registerNewPortCb(
       [this](rtmidiadapt::PortIndex index,
              const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          if (devOnUsbPort.getMidiPort().rfind("Midi Through", 0) == 0)
          {
             return;
          }
          spdlog::info("--> input added: {}", devOnUsbPort.getMidiPort());
          auto pMidiIn =
              createMidi<MusicDevice::MidiInput, midi::UsbMidiIn>(index);
          if (!pMidiIn)
          {
             spdlog::error("Could not create pMidiIn");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(Loader::Direction::IN, devOnUsbPort);
          switch (resType)
          {
             case Loader::ResultType::MusicDevice:
             {
                break;
             }
             case Loader::ResultType::NotFound:
             {
                break;
             }
             case Loader::ResultType::MarkedUnused:
             {
                spdlog::info(
                    "Marked UNUSED: {} | {} | {}", devOnUsbPort.getDeviceName(),
                    devOnUsbPort.getMidiPort(), devOnUsbPort.getUsbPortName());
                return;
             }
          }
          m_loader.markAsUsed(Loader::Direction::IN, devOnUsbPort);
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());

          auto pDescr = m_dataHolder.getDescription(deviceId.deviceName());
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
                       m_dataHolder.getDevicePresets(deviceId.deviceName())
                           ->savePreset(engineIdx, presetName,
                                        std::move(preset));
                       m_dataHolder.soundDevicesPresetChanged(sound::preset::Id(
                           {deviceId.deviceName(), engineIdx, presetName}));
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
          spdlog::info("--> output added: {}", devOnUsbPort.getMidiPort());
          auto pMidiOut =
              createMidi<MusicDevice::MidiOutput, midi::UsbMidiOut>(index);
          if (!pMidiOut)
          {
             spdlog::error("Could not create pMidiOut");
             return;
          }
          const auto [resType, deviceName] =
              m_loader.getMatchType(Loader::Direction::OUT, devOnUsbPort);
          switch (resType)
          {
             case Loader::ResultType::MusicDevice:
             {
                break;
             }
             case Loader::ResultType::NotFound:
             {
                break;
             }
             case Loader::ResultType::MarkedUnused:
             {
                spdlog::info(
                    "Marked UNUSED: {} | {} | {}", devOnUsbPort.getDeviceName(),
                    devOnUsbPort.getMidiPort(), devOnUsbPort.getUsbPortName());
                return;
             }
          }
          m_loader.markAsUsed(Loader::Direction::OUT, devOnUsbPort);
          const MusicDeviceId deviceId(deviceName,
                                       devOnUsbPort.getUsbPortName());
          auto pDescr = m_dataHolder.getDescription(deviceId.deviceName());
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
                       m_dataHolder.getDevicePresets(deviceId.deviceName())
                           ->savePreset(engineIdx, presetName,
                                        std::move(preset));
                       m_dataHolder.soundDevicesPresetChanged(sound::preset::Id(
                           {deviceId.deviceName(), engineIdx, presetName}));
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
          spdlog::info("<-- input removed: {}", devOnUsbPort.getMidiPort());
          m_loader.markAsUnused(Loader::Direction::IN, devOnUsbPort);
          const auto [resType, deviceName] =
              m_loader.getMatchType(Loader::Direction::IN, devOnUsbPort);
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
                    MidiHolder::Id(devOnUsbPort.getMidiPort(),
                                   devOnUsbPort.getUsbPortName()));
       },
       {{}, {IGNORED_DEVICES}, false});

   midi::PortNotifiers::instance().outputs.registerRemovedPortCb(
       [this](const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort) {
          spdlog::info("<-- output removed: {}", devOnUsbPort.getMidiPort());
          m_loader.markAsUnused(Loader::Direction::OUT, devOnUsbPort);
          const auto [resType, deviceName] =
              m_loader.getMatchType(Loader::Direction::OUT, devOnUsbPort);
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
                    MidiHolder::Id(devOnUsbPort.getMidiPort(),
                                   devOnUsbPort.getUsbPortName()));
       },
       {{}, {IGNORED_DEVICES}, false});
}

void Factory::createVirtualMidiDevices() noexcept
{
   /*
   {
      const std::string virtMidiInPortName = "nomidi-virt";
      spdlog::info("-->Virtual Midi Input added: {}", virtMidiInPortName);
      auto pMidiIn = createVirtualMidi<MusicDevice::MidiInput, midi::UsbMidiIn>(
          virtMidiInPortName);
      if (!pMidiIn)
      {
         spdlog::error("Could not create pMidiIn");
         return;
      }
      const MusicDeviceId deviceId(
          pMidiIn->medium().getDevicePortName(),
          pMidiIn->medium().getHostConnectorPortName());
      auto pDescr = m_dataHolder.getDescription(deviceId.deviceName);
      fillActionQueueForMidiIn(deviceId, std::move(pMidiIn));
   }
   */
   {
      const std::string virtMidiOutPortName = "nomidi-virt";
      spdlog::info("-->Virtual Midi Output added: {}", virtMidiOutPortName);
      auto pMidiOut =
          createVirtualMidi<MusicDevice::MidiOutput, midi::UsbMidiOut>(
              virtMidiOutPortName);
      if (!pMidiOut)
      {
         spdlog::error("Could not create pMidiOut");
         return;
      }
      const MusicDeviceId deviceId(
          pMidiOut->medium().getDevicePortName(),
          pMidiOut->medium().getHostConnectorPortName());
      auto pDescr = m_dataHolder.getDescription(deviceId.deviceName());
      fillActionQueueForMidiOut(deviceId, std::move(pMidiOut));
   }
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
                    m_dataHolder.getDescription(lastDeviceId.deviceName()),
                    m_dataHolder.getDevicePresets(lastDeviceId.deviceName()),
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

void Factory::fillActionQueueForMidiIn(
    const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn)
{
   util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
       .push(HandleMidiInInsert(), deviceId, pMidiIn,
             m_dataHolder.getDescription(deviceId.deviceName()),
             m_dataHolder.getDevicePresets(deviceId.deviceName()),
             m_dataHolder.getActualDevicePresetNames(deviceId));
   m_loader.forFirstDeviceInChain(
       deviceId, [this, pMidiIn](const MusicDeviceId& nextDeviceId,
                                 uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiInInsertChained(), nextDeviceId, pMidiIn,
                    m_dataHolder.getDescription(nextDeviceId.deviceName()),
                    m_dataHolder.getDevicePresets(nextDeviceId.deviceName()),
                    m_dataHolder.getActualDevicePresetNames(nextDeviceId));
       });
}

void Factory::fillActionQueueForMidiOut(
    const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut)
{
   util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
       .push(HandleMidiOutInsert(), deviceId, pMidiOut,
             m_dataHolder.getDescription(deviceId.deviceName()),
             m_dataHolder.getDevicePresets(deviceId.deviceName()),
             m_dataHolder.getActualDevicePresetNames(deviceId));
   m_loader.forEachDeviceInChain(
       deviceId, [this, pMidiOut](const MusicDeviceId& nextDeviceId,
                                  uint8_t midiVoiceOffset) {
          util::itc::ActionSender(m_actionQueue, m_musicDeviceInserter)
              .push(HandleMidiOutInsertChained(), nextDeviceId, pMidiOut,
                    m_dataHolder.getDescription(nextDeviceId.deviceName()),
                    m_dataHolder.getDevicePresets(nextDeviceId.deviceName()),
                    m_dataHolder.getActualDevicePresetNames(nextDeviceId));
       });
}