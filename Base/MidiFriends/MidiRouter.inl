#include "MidiRouter.h"

namespace midi
{
template<typename Msg>
inline auto createVoiceMsgOnChannel(const Msg& msg, int channel) noexcept
{
}

template<>
inline auto createVoiceMsgOnChannel<midi::Message<NoteOn>>(
   const Message<NoteOn>& msg, int channel) noexcept
{
   return Message<NoteOn>(channel, msg.noteNumber(), msg.velocity());
}

template<>
inline auto createVoiceMsgOnChannel<midi::Message<NoteOff>>(
   const Message<NoteOff>& msg, int channel) noexcept
{
   return Message<NoteOff>(channel, msg.noteNumber(), msg.velocity());
}

template<>
inline auto createVoiceMsgOnChannel<Message<AfterTouchPoly>>(
   const Message<AfterTouchPoly>& msg, int channel) noexcept
{
   return Message<AfterTouchPoly>(channel, msg.noteNumber(), msg.pressure());
}

template<>
inline auto createVoiceMsgOnChannel<Message<ControlChange>>(
   const Message<ControlChange>& msg, int channel) noexcept
{
   return Message<ControlChange>(channel, msg.controllerNumber(),
                                 msg.controllerValue());
}

template<>
inline auto createVoiceMsgOnChannel<Message<ProgramChange>>(
   const Message<ProgramChange>& msg, int channel) noexcept
{
   return Message<ProgramChange>(channel, msg.programNumber());
}

template<>
inline auto createVoiceMsgOnChannel<Message<AfterTouchChannel>>(
   const Message<AfterTouchChannel>& msg, int channel) noexcept
{
   return Message<AfterTouchChannel>(channel, msg.value());
}

template<>
inline auto createVoiceMsgOnChannel<Message<PitchBend>>(
   const Message<PitchBend>& msg, int channel) noexcept
{
   return Message<PitchBend>(channel, msg.value());
}

} // namespace midi

namespace base::midifriends
{
template<typename Msg>
void Router::handleVoiceMsg(
   const RoutingDataSpecialized::ChannelMap& channelMap, const Msg& msg,
   musicDevice::MusicDevice::MidiOutput& midiOut) noexcept
{
   const auto map = channelMap[msg.channel() - 1];
   if (0 == map)
      return;
   for (int x = 0; x < sizeof(map) * 8; ++x)
   {
      if (map & (1 << x))
      {
         midiOut.send(midi::createVoiceMsgOnChannel(msg, x + 1));
      }
   }
}

inline const RoutingData* Router::getRoutingData(
   const musicDevice::MidiHolder::Id& source,
   const musicDevice::MidiHolder::Id& dest) const noexcept
{
   const auto itSrc = m_routingData.find(source);
   if (itSrc == m_routingData.end())
   {
      return nullptr;
   }
   const auto itDst = itSrc->second.find(dest);
   if (itDst == itSrc->second.end())
   {
      return nullptr;
   }
   return &itDst->second;
}

inline RoutingData* Router::getRoutingData(
   const musicDevice::MidiHolder::Id& source,
   const musicDevice::MidiHolder::Id& dest) noexcept
{
   const auto itSrc = m_routingData.find(source);
   if (itSrc == m_routingData.end())
   {
      return nullptr;
   }
   const auto itDst = itSrc->second.find(dest);
   if (itDst == itSrc->second.end())
   {
      return nullptr;
   }
   return &itDst->second;
}

} // namespace base::midifriends
