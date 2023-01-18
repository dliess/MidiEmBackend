#ifndef BASE_MUSIC_DEVICE_INSERTER_H
#define BASE_MUSIC_DEVICE_INSERTER_H

#include <string>

#include "MidiHolder.h"
#include "MusicDevice.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "MusicDeviceId.h"
#include "itcQueue.h"

namespace base::musicDevice
{
struct Holder;
}
namespace base::musicDevice
{
struct DataHolder;
}

namespace base::musicDevice::factory
{
class MusicDeviceInserter
{
public:
   MusicDeviceInserter(Holder& rRtDataHolder, DataHolder& rDataHolder,
                       const std::string& resourceRootDir) noexcept;
   void HandleMidiInInsert(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames);
   void HandleMidiOutInsert(
       MusicDeviceId deviceId,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames);
   void HandleMidiInInsertChained(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames);
   void HandleMidiOutInsertChained(
       MusicDeviceId deviceId,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames);
   void EraseFromDevices(const MusicDeviceId& deviceId);
   void EraseFromMidiInHolder(const MidiHolder::Id& holderId);
   void EraseFromMidiOutHolder(const MidiHolder::Id& holderId);
   void HandleDeviceInsertChained(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames,
       uint8_t midiVoiceOffset);

    void invokeInserterQueueActions();

private:
   struct HandleMidiInInsert
   {
   };
   struct HandleMidiOutInsert
   {
   };
   struct HandleMidiInInsertChained
   {
   };
   struct HandleMidiOutInsertChained
   {
   };
   struct EraseFromDevices
   {
   };

   struct AddToMidiInHolder
   {
   };
   struct AddToMidiOutHolder
   {
   };
   struct EraseFromMidiInHolder
   {
   };
   struct EraseFromMidiOutHolder
   {
   };
   struct HandleDeviceInsertChained
   {
   };

   void action(EraseFromDevices, const MusicDeviceId& deviceId);
   void action(AddToMidiInHolder,
               std::shared_ptr<MusicDevice::MidiInput> pMidiIn);
   void action(AddToMidiOutHolder,
               std::shared_ptr<MusicDevice::MidiOutput> pMidiOut);
   void action(EraseFromMidiInHolder, const MidiHolder::Id& holderId);
   void action(EraseFromMidiOutHolder, const MidiHolder::Id& holderId);

   Holder& m_rRtDataHolder;
   DataHolder& m_rDataHolder;
   std::string m_resourceRootDir;
   util::itc::Queue m_actionQueue;
   bool findOrCreateDevice(
       const MusicDeviceId& deviceId,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames,
       std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut) noexcept;
   std::shared_ptr<MusicDevice> createMusicDevice(
       const MusicDeviceId& deviceId, const MusicDeviceId& midiDeviceId,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::DataHolder::ActualPresetNames>
           pActualPresetNames);
   static MusicDeviceId getMidiDevIdFrom(
       const std::shared_ptr<MusicDevice::MidiInput>& pMidiIn,
       const std::shared_ptr<MusicDevice::MidiOutput>& pMidiOut) noexcept;

   void addMidiToMdWithUUID(const util::Identifiable::UUID& uuid,
                            std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
                            std::shared_ptr<MusicDevice::MidiOutput> pMidiOut);
   void insertMDandMidiToMdWithUUID(
       std::shared_ptr<MusicDevice> pMusicDevice,
       std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       int midiVoiceOffset = 0);
};

}   // namespace base::musicDevice::factory

#endif