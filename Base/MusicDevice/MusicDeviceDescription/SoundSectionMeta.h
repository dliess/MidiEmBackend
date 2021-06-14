#ifndef SOUND_SECTION_META_H
#define SOUND_SECTION_META_H

#include "SoundSection.h"
#include "Meta.h"

namespace base::musicDevice::description::sound
{
#include "JsonCastNamespaceFix.h"
}

namespace meta
{

template<>
inline auto registerMembers<base::musicDevice::description::sound::ParameterId>()
{
   return members(
      member("engineId", &base::musicDevice::description::sound::ParameterId::engineId),
      member("parameterId", &base::musicDevice::description::sound::ParameterId::parameterId)
   );
}
template<>
inline auto getClassNameOrIndex<base::musicDevice::description::sound::ParameterId>(int i) noexcept
{
   return "base::musicDevice::description::sound::ParameterId";
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::NoteRange>()
{
   return members(
      member("from", &base::musicDevice::description::sound::NoteRange::from),
      member("to", &base::musicDevice::description::sound::NoteRange::to)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::NoteSettingsMidiRouting>()
{
   return members(
      member("destinationParameter", &base::musicDevice::description::sound::NoteSettingsMidiRouting::destinationParameter),
      member("mapping", &base::musicDevice::description::sound::NoteSettingsMidiRouting::mapping)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::NoteSettingsMidi>()
{
   return members(
      member("pitchRouting", &base::musicDevice::description::sound::NoteSettingsMidi::pitchRouting),
      member("velocityRouting", &base::musicDevice::description::sound::NoteSettingsMidi::velocityRouting)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::NoteSettings>()
{
   return members(
      member("noteRange", &base::musicDevice::description::sound::NoteSettings::noteRange),
      member("midi", &base::musicDevice::description::sound::NoteSettings::midi)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Voice>()
{
   return members(
      member("name", &base::musicDevice::description::sound::Voice::name),
      member("engineId", &base::musicDevice::description::sound::Voice::engineId),
      member("midiChannel", &base::musicDevice::description::sound::Voice::midiChannel),
      member("midiTriggerNoteNumber", &base::musicDevice::description::sound::Voice::midiTriggerNoteNumber)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Component>()
{
   return members(
      member("name", &base::musicDevice::description::sound::Component::name),
      member("type", &base::musicDevice::description::sound::Component::type)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterSourceRange>()
{
   return members(
      member("name", &base::musicDevice::description::sound::ParameterSourceRange::name),
      member("range", &base::musicDevice::description::sound::ParameterSourceRange::range)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterSourceMidi>()
{
   return members(
      member("id", &base::musicDevice::description::sound::ParameterSourceMidi::id),
      member("sourceRanges", &base::musicDevice::description::sound::ParameterSourceMidi::sourceRanges)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterSource>()
{
   return members(
      member("midi", &base::musicDevice::description::sound::ParameterSource::midi)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Parameter>()
{
   return members(
      member("name", &base::musicDevice::description::sound::Parameter::name),
      member("type", &base::musicDevice::description::sound::Parameter::type),
      member("component", &base::musicDevice::description::sound::Parameter::component),
      member("role", &base::musicDevice::description::sound::Parameter::role),
      member("description", &base::musicDevice::description::sound::Parameter::description),
      member("source", &base::musicDevice::description::sound::Parameter::source)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Global>()
{
   return members(
      member("midiChannel", &base::musicDevice::description::sound::Global::midiChannel),
      member("components", &base::musicDevice::description::sound::Global::components),
      member("parameters", &base::musicDevice::description::sound::Global::parameters)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Engine>()
{
   return members(
      member("name", &base::musicDevice::description::sound::Engine::name),
      member("noteSettings", &base::musicDevice::description::sound::Engine::noteSettings),
      member("components", &base::musicDevice::description::sound::Engine::components),
      member("parameters", &base::musicDevice::description::sound::Engine::parameters)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::MidiCCAndValue>()
{
   return members(
      member("cc", &base::musicDevice::description::sound::MidiCCAndValue::cc),
      member("value", &base::musicDevice::description::sound::MidiCCAndValue::value)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterDumpRequest>()
{
   return members(
      member("midiSysex", &base::musicDevice::description::sound::ParameterDumpRequest::midiSysex),
      member("midiMsg", &base::musicDevice::description::sound::ParameterDumpRequest::midiMsg)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Section>()
{
   return members(
      member("defaultInstrumentType", &base::musicDevice::description::sound::Section::defaultInstrumentType),
      member("voices", &base::musicDevice::description::sound::Section::voices),
      member("global", &base::musicDevice::description::sound::Section::global),
      member("engines", &base::musicDevice::description::sound::Section::engines),
      member("parameterDumpRequest", &base::musicDevice::description::sound::Section::parameterDumpRequest),
      member("pitchBendFactor", &base::musicDevice::description::sound::Section::pitchBendFactor)
   );
}

} // namespace meta


#endif