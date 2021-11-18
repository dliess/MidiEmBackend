#ifndef SOUND_DEVICE_PRESET_FETCHER_H
#define SOUND_DEVICE_PRESET_FETCHER_H

#include <memory>

#include "DoubleBufferedMessageDrain.h"
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MusicDeviceId.h"
#include "MusicDeviceDescription.h"

namespace base::musicDevice::sound
{
class PresetFetcher
{
public:
   PresetFetcher(std::shared_ptr<description::Description> pDescription) noexcept;
   using MidiInput  = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   const MusicDeviceId& musicDeviceId() const noexcept;
   void addMidiIn(std::shared_ptr<MidiInput> pMidiIn) noexcept;
   void addMidiOut(std::shared_ptr<MidiOutput> pMidiOut) noexcept;
   std::shared_ptr<MidiInput> hijackMidiIn() noexcept;
   std::shared_ptr<MidiOutput> hijackMidiOut() noexcept;
   [[nodiscard]] bool hasMidiInAndOut() const noexcept;
   void fetchPresets();

private:
   std::shared_ptr<description::Description> m_pDescription;
   std::shared_ptr<MidiInput> m_pMidiIn;
   std::shared_ptr<MidiOutput> m_pMidiOut;
};

}   // namespace base::musicDevice::sound

#endif