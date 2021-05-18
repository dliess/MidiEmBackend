#ifndef MUSIC_DEVICE_H
#define MUSIC_DEVICE_H

#include <memory>
#include <string>
#include <optional>
#include <SoundHandler.h>
#include <ControllerHandler.h>
#include "MusicDeviceId.h"
#include "DoubleBufferedMessageDrain.h"
#include "MusicDeviceSequencer.h"
#include "Identifiable.h"

namespace midi { class IMidiInMedium; }
namespace midi { class IMidiOutMedium; }
namespace midi { template<typename T> class Midi1Input; }
namespace midi { class Midi1Output; }
namespace midi { class Router; }
namespace midi { class Dumper; }

namespace base::musicDevice
{

namespace description { struct Description; }
namespace sound { class SoundPresets; }
struct MusicDevice : public util::Identifiable
{
   MusicDevice(MusicDeviceId deviceId, const std::string& resourceRootDir,
               std::shared_ptr<description::Description> descr,
               std::shared_ptr<sound::SoundPresets> soundPresets) noexcept;
   ~MusicDevice() noexcept;
   MusicDevice(const MusicDevice& other) = delete;
   MusicDevice& operator=(const MusicDevice& other) = delete;
   MusicDevice(MusicDevice&& other) noexcept;
   MusicDeviceId deviceId() const noexcept;

   using MidiInput = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   void initMidiIn(std::shared_ptr<MidiInput> pMidiInput) noexcept;
   void initMidiOut(std::shared_ptr<MidiOutput> pMidiOutput) noexcept;

   std::shared_ptr<description::Description> description() const noexcept;

   std::optional<sound::SoundHandler>           soundHandler;
   std::optional<controller::ControllerHandler> controllerHandler;
   std::optional<Sequencer> sequencer;

   friend midi::Router;
   friend midi::Dumper;

private:
   MusicDeviceId m_deviceId;
   PluginHandler m_pluginHandler;
   std::shared_ptr<description::Description> m_pDescr;
};

} // namespace base::musicDevice
#endif