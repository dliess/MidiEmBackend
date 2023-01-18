#include "MusicDeviceInserter.h"
#include "MusicDeviceHolder.h"

using namespace base::musicDevice;
using namespace base::musicDevice::factory;

MusicDeviceInserter::MusicDeviceInserter(
    Holder& rHolder, const std::string& resourceRootDir) noexcept :
    m_rHolder(rHolder), m_resourceRootDir(resourceRootDir)
{
}

std::shared_ptr<MusicDevice> MusicDeviceInserter::findOrCreateDevice(
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
         spdlog::error("Failed to add midi input medium'{}' {}",
                       deviceId.toStr(), e.what());
         return nullptr;
      }
   }
   return pMusicDevice;
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


void MusicDeviceInserter::action(
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

void MusicDeviceInserter::action(
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

void MusicDeviceInserter::action(
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

void MusicDeviceInserter::action(
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

void MusicDeviceInserter::action(EraseFromDevices,
                                          const MusicDeviceId& deviceId)
{
   m_rHolder.musicDevices.eraseByDeviceId(deviceId);
}

void MusicDeviceInserter::action(EraseFromMidiInHolder,
                                          const MidiHolder::Id& holderId)
{
   m_rHolder.midiHolder.removeMidiIn(holderId);
}

void MusicDeviceInserter::action(EraseFromMidiOutHolder,
                                          const MidiHolder::Id& holderId)
{
   m_rHolder.midiHolder.removeMidiOut(holderId);
}

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

void MusicDeviceInserter::action(
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