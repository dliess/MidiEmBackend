#ifndef MUSIC_DEVICE_FACTORY_H
#define MUSIC_DEVICE_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "DeviceDescriptionLoader.h"
#include "MusicDeviceLoader.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "MidiHolder.h"
#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "RtMidiAdaptTypes.h"
#include "itcQueue.h"
#include "PresetFetcher.h"

namespace base::musicDevice
{
struct Holder;

namespace description
{
struct Description;
}
namespace sound::preset
{
class DevicePresets;
}

namespace factory
{


class Factory
{
public:
   Factory(Holder& rMusicDeviceHolder, const std::string& resourceRootDir);

   void invokeInserterQueueActions();

   std::string getAllDevicesAsJson() const;
   void loadMusicDeviceToChain(const MusicDeviceId& chainRoot,
                               const MusicDeviceName& deviceName,
                               uint8_t midiVoiceOffset);
   void removeLastMusicDeviceFromChain(const MusicDeviceId& chainRoot);
   inline const DataHolder& dataHolder() const noexcept;
   inline DataHolder& dataHolder() noexcept;

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
   void insertMusicDeviceDummies();
#endif
private:
   Holder& m_rHolder;
   DataHolder m_dataHolder;
   Loader m_loader;
   std::unordered_map<MusicDeviceId, sound::PresetFetcher> m_soundPresetFetchers;
   util::itc::Queue m_actionQueue;
   void fillActionQueueForMidiIn(const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiInput> pMidiIn);
   void fillActionQueueForMidiOut(const MusicDeviceId& deviceId,
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOut);

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
      void action(
          HandleMidiInInsert, MusicDeviceId deviceId,
          std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets);
      void action(
          HandleMidiOutInsert, MusicDeviceId deviceId,
          std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets);
      void action(
          HandleMidiInInsertChained, MusicDeviceId deviceId,
          std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets);
      void action(
          HandleMidiOutInsertChained, MusicDeviceId deviceId,
          std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets);
      void action(EraseFromDevices, const MusicDeviceId& deviceId);
      void action(EraseFromMidiInHolder, const MidiHolder::Id& holderId);
      void action(EraseFromMidiOutHolder, const MidiHolder::Id& holderId);
      void action(
          HandleDeviceInsertChained, MusicDeviceId deviceId,
          std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
          std::shared_ptr<MusicDevice::MidiOutput> pMidiOut,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets,
          uint8_t midiVoiceOffset);

      Holder& m_rHolder;
      const std::string m_resourceRootDir;
      std::shared_ptr<MusicDevice> findOrCreateDevice(
          const MusicDeviceId& deviceId,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets,
          std::shared_ptr<MusicDevice::MidiInput> pMidiIn,
          std::shared_ptr<MusicDevice::MidiOutput>
              pMidiOut) noexcept;
      std::shared_ptr<MusicDevice> createMusicDevice(
          const MusicDeviceId& deviceId,
          std::shared_ptr<description::Description> pDescr,
          std::shared_ptr<sound::preset::DevicePresets> pPresets);
   };
   MusicDeviceInserter m_musicDeviceInserter;

   template <typename MidiType, typename MidiMediumType>
   static std::shared_ptr<MidiType> createMidi(
       rtmidiadapt::PortIndex index) noexcept;

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
   void addDummy(const std::string& deviceName) noexcept;
#endif
};

}   // namespace factory
}   // namespace base::musicDevice

#include "MusicDeviceFactory.tcc"

#endif