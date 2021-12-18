#ifndef MUSIC_DEVICE_H
#define MUSIC_DEVICE_H

#include <ControllerHandler.h>
#include <SoundHandler.h>

#include <memory>
#include <optional>
#include <string>

#include "DoubleBufferedMessageDrain.h"
#include "Identifiable.h"
#include "MusicDeviceId.h"
#include "MusicDeviceSequencer.h"

namespace midi
{
class IMidiInMedium;
}
namespace midi
{
class IMidiOutMedium;
}
namespace midi
{
template <typename T> class Midi1Input;
}
namespace midi
{
class Midi1Output;
}
namespace midi
{
class Router;
}
namespace midi
{
class Dumper;
}

namespace base::musicDevice
{
namespace description
{
struct Description;
}
namespace sound::preset
{
class DevicePresets;
}
struct MusicDevice : public util::Identifiable
{
   MusicDevice(
       MusicDeviceId deviceId, MusicDeviceId midiMediumId,
       const std::string& resourceRootDir,
       std::shared_ptr<description::Description> descr,
       std::shared_ptr<sound::preset::DevicePresets> soundPresets,
       std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept;
   ~MusicDevice() noexcept;
   MusicDevice(const MusicDevice& other) = delete;
   MusicDevice& operator=(const MusicDevice& other) = delete;
   MusicDevice(MusicDevice&& other) noexcept;
   MusicDeviceId deviceId() const noexcept;

   using MidiInput  = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   void initMidiIn(std::shared_ptr<MidiInput> pMidiInput,
                   uint8_t midiVoiceOffset = 0) noexcept;
   void initMidiOut(std::shared_ptr<MidiOutput> pMidiOutput,
                    uint8_t midiVoiceOffset = 0) noexcept;
   const MusicDeviceId& mediumId() const;

   std::shared_ptr<description::Description> description() const noexcept;

   std::optional<sound::SoundHandler> soundHandler;
   std::optional<controller::ControllerHandler> controllerHandler;
   std::optional<Sequencer> sequencer;

   friend midi::Router;
   friend midi::Dumper;

private:
   MusicDeviceId m_deviceId;
   MusicDeviceId m_mediumId;
   PluginHandler m_pluginHandler;
   std::shared_ptr<description::Description> m_pDescr;
};

}   // namespace base::musicDevice

#include "MusicDeviceMeta.h"

#endif