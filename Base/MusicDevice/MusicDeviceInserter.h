#ifndef BASE_MUSIC_DEVICE_INSERTER_H
#define BASE_MUSIC_DEVICE_INSERTER_H

#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "MidiHolder.h"
#include "MusicDeviceFactoryDataHolder.h"
#include <string>

namespace base::musicDevice { struct Holder; }

namespace base::musicDevice::factory
{

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
struct EraseFromMidiInHolder
{
};
struct EraseFromMidiOutHolder
{
};
struct HandleDeviceInsertChained
{
};


class MusicDeviceInserter
{
public:
   MusicDeviceInserter(Holder& rHolder,
                       const std::string& resourceRootDir) noexcept;
   void action(HandleMidiInInsert, MusicDeviceId deviceId,
               std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
               std::shared_ptr<description::Description> pDescr,
               std::shared_ptr<sound::preset::DevicePresets> pPresets,
               std::shared_ptr<factory::DataHolder::ActualPresetNames>
                   pActualPresetNames);
   void action(HandleMidiOutInsert, MusicDeviceId deviceId,
               std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
               std::shared_ptr<description::Description> pDescr,
               std::shared_ptr<sound::preset::DevicePresets> pPresets,
               std::shared_ptr<factory::DataHolder::ActualPresetNames>
                   pActualPresetNames);
   void action(HandleMidiInInsertChained, MusicDeviceId deviceId,
               std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
               std::shared_ptr<description::Description> pDescr,
               std::shared_ptr<sound::preset::DevicePresets> pPresets,
               std::shared_ptr<factory::DataHolder::ActualPresetNames>
                   pActualPresetNames);
   void action(HandleMidiOutInsertChained, MusicDeviceId deviceId,
               std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
               std::shared_ptr<description::Description> pDescr,
               std::shared_ptr<sound::preset::DevicePresets> pPresets,
               std::shared_ptr<factory::DataHolder::ActualPresetNames>
                   pActualPresetNames);
   void action(EraseFromDevices, const MusicDeviceId& deviceId);
   void action(EraseFromMidiInHolder, const MidiHolder::Id& holderId);
   void action(EraseFromMidiOutHolder, const MidiHolder::Id& holderId);
   void action(HandleDeviceInsertChained, MusicDeviceId deviceId,
               std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
               std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
               std::shared_ptr<description::Description> pDescr,
               std::shared_ptr<sound::preset::DevicePresets> pPresets,
               std::shared_ptr<factory::DataHolder::ActualPresetNames>
                   pActualPresetNames,
               uint8_t midiVoiceOffset);
private:
   Holder& m_rHolder;
   const std::string m_resourceRootDir;
   std::shared_ptr<MusicDevice> findOrCreateDevice(
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
};

}   // namespace base::musicDevice::factory

#endif