#ifndef MUSIC_DEVICE_MIDI_HOLDER_INL
#define MUSIC_DEVICE_MIDI_HOLDER_INL

#include "BeatTick.h"
#include <loguru.hpp>
namespace base::musicDevice
{
inline void MidiHolder::registerForInputAdded(CbIn cb) noexcept
{
   m_inputAddedCbs.emplace_back(cb);
}

inline void MidiHolder::registerForOutputAdded(CbOut cb) noexcept
{
   m_outputAddedCbs.emplace_back(cb);
}

inline void MidiHolder::registerForInputRemoved(CbRem cb) noexcept
{
   m_inputRemovedCbs.emplace_back(cb);
}

inline void MidiHolder::registerForOutputRemoved(CbRem cb) noexcept
{
   m_outputRemovedCbs.emplace_back(cb);
}

inline void MidiHolder::processMidiInBuffers()
{
   for (auto& midiInput : m_midiInputs) { midiInput->update(); }
}

inline void MidiHolder::addMidiIn(
    std::shared_ptr<MusicDevice::MidiInput> pMidiInput) noexcept
{
   for (auto& cb : m_inputAddedCbs) cb(pMidiInput);
   m_midiInputs.emplace_back(std::move(pMidiInput));
}

inline void MidiHolder::addMidiOut(
    std::shared_ptr<MusicDevice::MidiOutput> pMidiOutput) noexcept
{
   for (auto& cb : m_outputAddedCbs) cb(pMidiOutput);
   m_midiOutputs.emplace_back(std::move(pMidiOutput), 0, 0);
}

inline void MidiHolder::removeMidiIn(const Id& id) noexcept
{
   for (int i = 0; i < m_midiInputs.size(); ++i)
   {
      if (id == Id(m_midiInputs[i]->medium().getDeviceName(),
                   m_midiInputs[i]->medium().getPortName()))
      {
         for (auto& cb : m_inputRemovedCbs) { cb(id); }
         m_midiInputs[i].reset();
         m_midiInputs.erase(m_midiInputs.begin() + i);
      }
   }
}

inline void MidiHolder::removeMidiOut(const Id& id) noexcept
{
   for (int i = 0; i < m_midiOutputs.size(); ++i)
   {
      if (id == Id(m_midiOutputs[i].pMidiOut->medium().getDeviceName(),
                   m_midiOutputs[i].pMidiOut->medium().getPortName()))
      {
         for (auto& cb : m_outputRemovedCbs) { cb(id); }
         m_midiOutputs[i].pMidiOut.reset();
          m_midiOutputs.erase(m_midiOutputs.begin() + i);
      } 
   }
}

inline std::shared_ptr<MusicDevice::MidiInput> MidiHolder::getMidiIn(
    const Id& id) const noexcept
{
   for (auto& e : m_midiInputs)
   {
      const Id actId(e->medium().getDeviceName(), e->medium().getPortName());
      if (actId == id)
      {
         return e;
      }
   }
   return nullptr;
}

inline std::shared_ptr<MusicDevice::MidiOutput> MidiHolder::getMidiOut(
    const Id& id) const noexcept
{
   for (auto& e : m_midiOutputs)
   {
      const Id actId(e.pMidiOut->medium().getDeviceName(), e.pMidiOut->medium().getPortName());
      if (actId == id)
      {
         return e.pMidiOut;
      }
   }
   return nullptr;
}

inline void MidiHolder::midiClock(double beatsDelta) noexcept
{
   constexpr int MIDI_PPQ               = 24;
   static double midiClockTickNotHandled = 0;
   midiClockTickNotHandled += (beatsDelta * MIDI_PPQ);
   const int ticksToSend = static_cast<int>(midiClockTickNotHandled);
   midiClockTickNotHandled = midiClockTickNotHandled - ticksToSend;
   for (int i = 0; i < ticksToSend; ++i)
   {
      for (auto& e : m_midiOutputs) { e.pMidiOut->send(midi::Message<midi::Clock>()); }
   }
}

}   // namespace base::musicDevice

#endif