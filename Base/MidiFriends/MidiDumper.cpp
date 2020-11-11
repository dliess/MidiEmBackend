#include "MidiDumper.h"

#include "Midi.h"

using namespace base::midifriends;

Dumper::Dumper(musicDevice::MidiHolder& rMidiHolder) noexcept
{
   rMidiHolder.registerForInputAdded(
      [this](
         const std::shared_ptr<musicDevice::MusicDevice::MidiInput>& pMidiIn) {
         const musicDevice::MidiHolder::Id id(pMidiIn->medium().getDeviceName(),
                                              pMidiIn->medium().getPortName());
         pMidiIn->registerMidiInCb(
            [this, &id](const midi::MidiMessage& midiMsg) {
               handleMidiIn(id, midiMsg);
            }
         );
      }
   );

   rMidiHolder.registerForInputRemoved(
      [this](const musicDevice::MidiHolder::Id& id) {
         m_data.erase(id);
      }
   );
}

void Dumper::handleMidiIn(const musicDevice::MidiHolder::Id& id,
                          const midi::MidiMessage& midiMsg) noexcept
{
   auto it = m_data.find(id);
   assert(it != m_data.end());
   auto& data = it->second;
   if (mpark::holds_alternative<midi::Message<midi::Clock>>(midiMsg))
   {
      data.clockReceived = true;
   }
   else
   {
      {
         std::unique_lock lock(data.ringBufLock);
         data.ringBuf.push(midiMsg);
      }
      if (m_ringBufChangedCb)
      {
         m_ringBufChangedCb(id);
      }
   }
}

Dumper::RingBuf Dumper::getRingBuf(const musicDevice::MidiHolder::Id& id) const noexcept
{
   auto it = m_data.find(id);
   assert(it != m_data.end());
   auto& data = it->second;

   std::unique_lock lock(data.ringBufLock);
   return data.ringBuf;
}

bool Dumper::getClockReceived(const musicDevice::MidiHolder::Id& id) const noexcept
{
   const auto it = m_data.find(id);
   assert(it != m_data.end());
   const auto& data = it->second;
   return data.clockReceived;
}

void Dumper::resetClockReceived(const musicDevice::MidiHolder::Id& id) noexcept
{
   auto it = m_data.find(id);
   assert(it != m_data.end());
   auto& data = it->second;
   data.clockReceived = false;
}

void Dumper::registerRingbufChangedCb(CB cb) noexcept
{
   m_ringBufChangedCb = cb;
}