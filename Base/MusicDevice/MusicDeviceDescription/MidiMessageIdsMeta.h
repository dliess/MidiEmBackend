#ifndef MIDI_MESSAGE_IDS_META_H
#define MIDI_MESSAGE_IDS_META_H

#include "MidiMessageIds.h"
#include "JsonCast.h"
#include "Meta.h"

namespace meta
{
template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::NoteOff>>(int i) noexcept
{
   return "MidiNoteOff";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::NoteOff>>()
{
   return members(member("note", &midi::MidiMsgId<midi::NoteOff>::note));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::NoteOn>>(int i) noexcept
{
   return "MidiNoteOn";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::NoteOn>>()
{
   return members(member("note", &midi::MidiMsgId<midi::NoteOn>::note));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::ControlChange>>(int i) noexcept
{
   return "MidiControlChange";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::ControlChange>>()
{
   return members(member("id", &midi::MidiMsgId<midi::ControlChange>::id));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::ControlChangeHighRes>>(
   int i) noexcept
{
   return "MidiControlChangeHighRes";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::ControlChangeHighRes>>()
{
   return members(
      member("idMsb", &midi::MidiMsgId<midi::ControlChangeHighRes>::idMsb),
      member("idLsb", &midi::MidiMsgId<midi::ControlChangeHighRes>::idLsb));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::NRPN>>(int i) noexcept
{
   return "MidiNRPN";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::NRPN>>()
{
   return members(member("idMsb", &midi::MidiMsgId<midi::NRPN>::idMsb),
                  member("idLsb", &midi::MidiMsgId<midi::NRPN>::idLsb));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::RPN>>(int i) noexcept
{
   return "MidiRPN";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::RPN>>()
{
   return members(member("idMsb", &midi::MidiMsgId<midi::RPN>::idMsb),
                  member("idLsb", &midi::MidiMsgId<midi::RPN>::idLsb));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::AfterTouchPoly>>(int i) noexcept
{
   return "MidiAfterTouchPoly";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::AfterTouchPoly>>()
{
   return members(member("note", &midi::MidiMsgId<midi::AfterTouchPoly>::note));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::AfterTouchChannel>>(
   int i) noexcept
{
   return "MidiAfterTouchChannel";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::AfterTouchChannel>>()
{
   return members(member("dummy", &midi::MidiMsgId<midi::AfterTouchChannel>::dummy));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::PitchBend>>(int i) noexcept
{
   return "MidiPitchBend";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::PitchBend>>()
{
   return members(member("dummy", &midi::MidiMsgId<midi::PitchBend>::dummy));
}

template<>
inline auto getClassNameOrIndex<midi::MidiMsgId<midi::SystemExclusive>>(int i) noexcept
{
   return "SystemExclusive";
}
template<>
inline auto registerMembers<midi::MidiMsgId<midi::SystemExclusive>>()
{
   return members(member("dummy", &midi::MidiMsgId<midi::SystemExclusive>::dummy));
}


} // namespace meta

#endif