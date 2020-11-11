#ifndef MIDI_DUMPER_H
#define MIDI_DUMPER_H

#include <atomic>
#include <functional>
#include <mutex>
#include <unordered_map>

#include "MidiMessage.h"
#include "Ringbuffer.hpp"
#include "MidiHolder.h"

namespace base
{
namespace midifriends
{
class Dumper
{
public:
   Dumper(musicDevice::MidiHolder& rMidiHolder) noexcept;
   using RingBuf = util::Ringbuffer<midi::MidiMessage, 64>;
   RingBuf getRingBuf(const musicDevice::MidiHolder::Id& id) const noexcept;
   bool getClockReceived(const musicDevice::MidiHolder::Id& id) const noexcept;
   void resetClockReceived(const musicDevice::MidiHolder::Id& id) noexcept;
   using CB = std::function<void(const musicDevice::MidiHolder::Id&)>;
   void registerRingbufChangedCb(CB cb) noexcept;

private:
   struct Data
   {
      RingBuf ringBuf;
      std::atomic<bool> clockReceived{false};
      mutable std::mutex ringBufLock;
   };
   using DataMap = std::unordered_map<musicDevice::MidiHolder::Id, Data>;
   DataMap m_data;
   CB m_ringBufChangedCb;
   void handleMidiIn(const musicDevice::MidiHolder::Id& id,
                     const midi::MidiMessage& midiMsg) noexcept;
};

} // namespace midifriends
} // namespace base

#endif