#ifndef SOUND_DEVICE_PRESET_FETCHER_H
#define SOUND_DEVICE_PRESET_FETCHER_H

#include <memory>

#include "DoubleBufferedMessageDrain.h"
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "SoundSection.h"

namespace base::musicDevice::sound
{
class PresetFetcher
{
public:
   PresetFetcher(const description::sound::Section& rSoundSection) noexcept;
   using MidiInput  = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   void addMidiIn(std::shared_ptr<MidiInput> pMidiIn) noexcept;
   void addMidiOut(std::shared_ptr<MidiOutput> pMidiOut) noexcept;
   std::shared_ptr<MidiInput> hijackMidiIn() noexcept;
   std::shared_ptr<MidiOutput> hijackMidiOut() noexcept;
private:
   const description::sound::Section& m_rSoundSection;
   std::shared_ptr<MidiInput> m_pMidiIn;
   std::shared_ptr<MidiOutput> m_pMidiOut;
   void fetchPresets();
};

}   // namespace base::musicDevice::sound

#endif