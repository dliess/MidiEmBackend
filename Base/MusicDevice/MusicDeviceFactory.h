#ifndef MUSIC_DEVICE_FACTORY_H
#define MUSIC_DEVICE_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "DeviceDescriptionLoader.h"
#include "MidiHolder.h"
#include "MusicDevice.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "MusicDeviceId.h"
#include "MusicDeviceLoader.h"
#include "PresetFetcher.h"
#include "RtMidiAdaptTypes.h"
#include "itcQueue.h"
#include "MusicDeviceInserter.h"

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
   void createVirtualMidiDevices() noexcept;

private:
   Holder& m_rHolder;
   DataHolder m_dataHolder;
   Loader m_loader;
   std::unordered_map<MusicDeviceId, sound::PresetFetcher>
       m_soundPresetFetchers;
   util::itc::Queue m_actionQueue;
   void fillActionQueueForMidiIn(
       const MusicDeviceId& deviceId,
       std::shared_ptr<MusicDevice::MidiInput> pMidiIn);
   void fillActionQueueForMidiOut(
       const MusicDeviceId& deviceId,
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOut);

   MusicDeviceInserter m_musicDeviceInserter;

   template <typename MidiType, typename MidiMediumType>
   static std::shared_ptr<MidiType> createMidi(
       rtmidiadapt::PortIndex index) noexcept;

   template <typename MidiType, typename MidiMediumType>
   std::shared_ptr<MidiType> createVirtualMidi(std::string portName) noexcept;
};

}   // namespace factory
}   // namespace base::musicDevice

#include "MusicDeviceFactory.inl"

#endif