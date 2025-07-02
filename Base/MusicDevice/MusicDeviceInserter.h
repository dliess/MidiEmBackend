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
   MusicDeviceInserter(Holder& rRtDataHolder, MusicDevices& rDataHolder,
                       const std::string& resourceRootDir) noexcept;
   void handleMidiInInsert(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames);
   void handleMidiOutInsert(
       MusicDeviceId deviceId,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames);
   void handleMidiInInsertChained(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames);
   void handleMidiOutInsertChained(
       MusicDeviceId deviceId,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames);
   void eraseFromDevices(const MusicDeviceId& deviceId);
   void eraseFromMidiInHolder(const MidiHolder::Id& holderId);
   void eraseFromMidiOutHolder(const MidiHolder::Id& holderId);
   void handleDeviceInsertChained(
       MusicDeviceId deviceId, std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames,
       uint8_t midiVoiceOffset);

    void invokeQueueActions();

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

private:
   Holder& m_rRtDataHolder;
   MusicDevices& m_rDataHolder;
   std::string m_resourceRootDir;
   util::itc::Queue m_actionQueue;
   bool findOrCreateDevice(
       const MusicDeviceId& deviceId,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
           pActualPresetNames,
       std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut) noexcept;
   std::shared_ptr<MusicDevice> createMusicDevice(
       const MusicDeviceId& deviceId, const MusicDeviceId& midiDeviceId,
       std::shared_ptr<description::Description> pDescr,
       std::shared_ptr<sound::preset::DevicePresets> pPresets,
       std::shared_ptr<factory::MusicDevices::ActualPresetNames>
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
