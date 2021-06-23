#ifndef MUSIC_DEVICE_MIDI_HOLDER_H
#define MUSIC_DEVICE_MIDI_HOLDER_H

#include <vector>
#include <functional>
#include <memory>

#include "MusicDevice.h"

namespace base
{

namespace midifriends { class Dumper; }
namespace midifriends { class Router; }

namespace musicDevice
{

class MidiHolder
{
public:
   using Id = MusicDeviceId;
   inline void processMidiInBuffers();
   inline void addMidiIn(std::shared_ptr<MusicDevice::MidiInput> pMidiInput) noexcept;
   inline void addMidiOut(std::shared_ptr<MusicDevice::MidiOutput> pMidiOutput) noexcept;
   inline void removeMidiIn(const Id& id) noexcept;
   inline void removeMidiOut(const Id& id) noexcept;

   using CbIn = std::function<void(const std::shared_ptr<MusicDevice::MidiInput>&)>;
   using CbOut = std::function<void(const std::shared_ptr<MusicDevice::MidiOutput>&)>;
   using CbRem = std::function<void(const Id&)>;
   inline void registerForInputAdded(CbIn cb) noexcept;
   inline void registerForOutputAdded(CbOut cb) noexcept;
   inline void registerForInputRemoved(CbRem cb) noexcept;
   inline void registerForOutputRemoved(CbRem cb) noexcept;

   friend midifriends::Dumper;
   friend midifriends::Router;

   inline std::shared_ptr<MusicDevice::MidiInput> getMidiIn(const Id& id) const noexcept;
   inline std::shared_ptr<MusicDevice::MidiOutput> getMidiOut(const Id& id) const noexcept;

   inline void midiClock() noexcept;

private:
   std::vector<std::shared_ptr<MusicDevice::MidiInput>> m_midiInputs;
   std::vector<std::shared_ptr<MusicDevice::MidiOutput>> m_midiOutputs;
   std::vector<CbIn> m_inputAddedCbs;
   std::vector<CbOut> m_outputAddedCbs;
   std::vector<CbRem> m_inputRemovedCbs;
   std::vector<CbRem> m_outputRemovedCbs;
};

} // namespace musicDevice
} // namespace base

#include "MidiHolder.inl"

#endif