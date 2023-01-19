#include "MusicDeviceInserter.h"

#include "MusicDeviceFactoryDataHolder.h"
#include "MusicDeviceHolder.h"
#include "itcActionSender.h"

using namespace base::musicDevice;
using namespace base::musicDevice::factory;

MusicDeviceInserter::MusicDeviceInserter(
    Holder& rRtDataHolder, DataHolder& rDataHolder,
    const std::string& resourceRootDir) noexcept :
    m_rRtDataHolder(rRtDataHolder),
    m_rDataHolder(rDataHolder),
    m_resourceRootDir(resourceRootDir)
{
}

bool MusicDeviceInserter::findOrCreateDevice(
    const MusicDeviceId& deviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut) noexcept
{
   const auto uuid = m_rDataHolder.getUUIDByMdId(deviceId);
   if (uuid)
   {
      addMidiToMdWithUUID(*uuid, std::move(pMidiIn), std::move(pMidiOut));
   }
   else
   {
      try
      {
         std::shared_ptr<MusicDevice> pMusicDevice = createMusicDevice(
             deviceId, getMidiDevIdFrom(pMidiIn, pMidiOut), std::move(pDescr),
             std::move(pPresets), std::move(pActualPresetNames));
         m_rDataHolder.addUuid2MdId(pMusicDevice->id(), pMusicDevice.get());
         insertMDandMidiToMdWithUUID(std::move(pMusicDevice),
                                     std::move(pMidiIn), std::move(pMidiOut));
      }
      catch (std::exception& e)
      {
         spdlog::error("Failed to add midi input medium'{}' {}",
                       deviceId.toStr(), e.what());
         return false;
      }
   }
   return true;
}

void MusicDeviceInserter::addMidiToMdWithUUID(
    const util::Identifiable::UUID& uuid,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut)
{
   auto itCntrlDev = m_rRtDataHolder.musicDevices.find(uuid);
   assert(itCntrlDev != m_rRtDataHolder.musicDevices.end());
   if (pMidiIn)
   {
      itCntrlDev->second->initMidiIn(std::move(pMidiIn));
   }
   if (pMidiOut)
   {
      itCntrlDev->second->initMidiOut(std::move(pMidiOut));
   }
}

void MusicDeviceInserter::insertMDandMidiToMdWithUUID(
    std::shared_ptr<MusicDevice> pMusicDevice,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut, int midiVoiceOffset)
{
   // NOTE: Important to insert it first to trigger cb-signals in right
   // order
   m_rRtDataHolder.musicDevices.insert(
       std::make_pair(pMusicDevice->id(), pMusicDevice));
   if (pMidiIn)
   {
      pMusicDevice->initMidiIn(std::move(pMidiIn), midiVoiceOffset);
   }
   if (pMidiOut)
   {
      pMusicDevice->initMidiOut(std::move(pMidiOut), midiVoiceOffset);
   }
}

std::shared_ptr<MusicDevice> MusicDeviceInserter::createMusicDevice(
    const MusicDeviceId& deviceId, const MusicDeviceId& midiDeviceId,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   spdlog::info("Created Music Device {}", deviceId.toStr());
   auto pMusicDevice = std::make_shared<MusicDevice>(
       deviceId, midiDeviceId, m_resourceRootDir, std::move(pDescr),
       std::move(pPresets), std::move(pActualPresetNames));
   spdlog::info("uuid: [{}]", util::uuid2Str(pMusicDevice->id()));
   return std::move(pMusicDevice);
}

void MusicDeviceInserter::handleMidiInInsert(MusicDeviceId deviceId,
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
   util::itc::ActionSender(m_actionQueue, *this)
       .push(AddToMidiInHolder(), std::move(pMidiIn));
}

void MusicDeviceInserter::handleMidiOutInsert(
    MusicDeviceId deviceId,
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
   util::itc::ActionSender(m_actionQueue, *this)
       .push(AddToMidiOutHolder(), std::move(pMidiOut));
}

void MusicDeviceInserter::handleMidiInInsertChained(
    MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                      std::move(pActualPresetNames), std::move(pMidiIn),
                      nullptr);
}

void MusicDeviceInserter::handleMidiOutInsertChained(
    MusicDeviceId deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
    std::shared_ptr<description::Description> pDescr,
    std::shared_ptr<sound::preset::DevicePresets> pPresets,
    std::shared_ptr<factory::DataHolder::ActualPresetNames> pActualPresetNames)
{
   findOrCreateDevice(deviceId, std::move(pDescr), std::move(pPresets),
                      std::move(pActualPresetNames), nullptr,
                      std::move(pMidiOut));
}

void MusicDeviceInserter::eraseFromDevices(
                                 const MusicDeviceId& deviceId)
{
   auto mdId = m_rDataHolder.getUUIDByMdId(deviceId);
   if(mdId)
   {
      m_rDataHolder.removeEntryForUuid(*mdId);
   }
   util::itc::ActionSender(m_actionQueue, *this)
       .push(EraseFromDevices(), deviceId);
}

void MusicDeviceInserter::eraseFromMidiInHolder(const MidiHolder::Id& holderId)
{
   util::itc::ActionSender(m_actionQueue, *this)
       .push(EraseFromMidiInHolder(), holderId);
}

void MusicDeviceInserter::eraseFromMidiOutHolder(const MidiHolder::Id& holderId)
{
   util::itc::ActionSender(m_actionQueue, *this)
       .push(EraseFromMidiOutHolder(), holderId);
}

void MusicDeviceInserter::handleDeviceInsertChained(
    MusicDeviceId deviceId,
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
   m_rDataHolder.addUuid2MdId(pMusicDevice->id(), pMusicDevice.get());
   insertMDandMidiToMdWithUUID(std::move(pMusicDevice), std::move(pMidiIn),
                               std::move(pMidiOut), midiVoiceOffset);
}

void MusicDeviceInserter::invokeQueueActions() { m_actionQueue.popCallAll(); }

MusicDeviceId MusicDeviceInserter::getMidiDevIdFrom(
    const std::shared_ptr<MusicDevice::MidiInput>& pMidiIn,
    const std::shared_ptr<MusicDevice::MidiOutput>& pMidiOut) noexcept
{
   if (pMidiIn)
      return {pMidiIn->medium().getDevicePortName(),
              pMidiIn->medium().getHostConnectorPortName()};
   if (pMidiOut)
      return {pMidiOut->medium().getDevicePortName(),
              pMidiOut->medium().getHostConnectorPortName()};
   assert(false);
   return MusicDeviceId();
}

void MusicDeviceInserter::action(EraseFromDevices,
                                 const MusicDeviceId& deviceId)
{
   m_rRtDataHolder.musicDevices.eraseByDeviceId(deviceId);
}

void MusicDeviceInserter::action(AddToMidiInHolder,
                                 std::shared_ptr<MusicDevice::MidiInput> pMidiIn)
{
    m_rRtDataHolder.midiHolder.addMidiIn(std::move(pMidiIn));
}

void MusicDeviceInserter::action(AddToMidiOutHolder,
                                 std::shared_ptr<MusicDevice::MidiOutput> pMidiOut)
{
    m_rRtDataHolder.midiHolder.addMidiOut(std::move(pMidiOut));
}

void MusicDeviceInserter::action(EraseFromMidiInHolder,
                                 const MidiHolder::Id& holderId)
{
   m_rRtDataHolder.midiHolder.removeMidiIn(holderId);
}

void MusicDeviceInserter::action(EraseFromMidiOutHolder,
                                 const MidiHolder::Id& holderId)
{
   m_rRtDataHolder.midiHolder.removeMidiOut(holderId);
}