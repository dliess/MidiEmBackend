#ifndef MUSIC_DEVICE_MIDI_HOLDER_H
#define MUSIC_DEVICE_MIDI_HOLDER_H

#include <chrono>
#include <functional>
#include <memory>
#include <vector>

#include "MusicDevice.h"

namespace base
{
namespace midifriends
{
class Dumper;
}
namespace midifriends
{
class Router;
}

namespace musicDevice
{
class MidiHolder
{
public:
   using Id = MusicDeviceId;
   inline void processMidiInBuffers();
   inline void addMidiIn(
       std::shared_ptr<MusicDevice::MidiInput> pMidiInput) noexcept;
   inline void addMidiOut(
       std::shared_ptr<MusicDevice::MidiOutput> pMidiOutput) noexcept;
   inline void removeMidiIn(const Id& id) noexcept;
   inline void removeMidiOut(const Id& id) noexcept;

   using CbIn =
       std::function<void(const std::shared_ptr<MusicDevice::MidiInput>&)>;
   using CbOut =
       std::function<void(const std::shared_ptr<MusicDevice::MidiOutput>&)>;
   using CbRem = std::function<void(const Id&)>;
   inline void registerForInputAdded(CbIn cb) noexcept;
   inline void registerForOutputAdded(CbOut cb) noexcept;
   inline void registerForInputRemoved(CbRem cb) noexcept;
   inline void registerForOutputRemoved(CbRem cb) noexcept;

   friend midifriends::Dumper;
   friend midifriends::Router;

   [[nodiscard]] inline std::shared_ptr<MusicDevice::MidiInput> getMidiIn(
       const Id& id) const noexcept;
   [[nodiscard]] inline std::shared_ptr<MusicDevice::MidiOutput> getMidiOut(
       const Id& id) const noexcept;

   struct MidiOutEntry
   {
      template <typename T>
      MidiOutEntry(T&& ptr) :
          pMidiOut(std::forward<T>(ptr))
      {
      }
      std::shared_ptr<MusicDevice::MidiOutput> pMidiOut;
      int offsetSpeedBpm{0};
      double accumulatedOffsetBeats{0};
      double midiClockTickNotHandled{0};
      std::chrono::microseconds accumulatedOffsetTimeUs{0};
      double lateStartPressBeats{0};
   };
   [[nodiscard]] inline MidiOutEntry* midiOutEntry(const Id& id) noexcept;

   inline void midiClock(double beatsDelta,
                         const std::chrono::microseconds& deltaTime =
                             std::chrono::microseconds::zero()) noexcept;

private:
   std::vector<std::shared_ptr<MusicDevice::MidiInput>> m_midiInputs;
   std::vector<MidiOutEntry> m_midiOutputs;
   std::vector<CbIn> m_inputAddedCbs;
   std::vector<CbOut> m_outputAddedCbs;
   std::vector<CbRem> m_inputRemovedCbs;
   std::vector<CbRem> m_outputRemovedCbs;
};

}   // namespace musicDevice
}   // namespace base

#include "MidiHolder.inl"

#endif