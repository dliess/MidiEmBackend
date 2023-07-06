#ifndef SOUND_SECTION_META_H
#define SOUND_SECTION_META_H

#include "SoundSection.h"
#include "Meta.h"

namespace meta
{

template<>
inline auto registerMembers<base::musicDevice::description::sound::ParameterDumpAnswer>()
{
   return members(
      member("sysexDescriptors", &base::musicDevice::description::sound::ParameterDumpAnswer::sysexDescriptors)
   );
}

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
      member("midi", &base::musicDevice::description::sound::NoteSettings::midi),
      member("defaultTriggerNote", &base::musicDevice::description::sound::NoteSettings::defaultTriggerNote)
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
      member("role", &base::musicDevice::description::sound::Component::role),
      member("constParams", &base::musicDevice::description::sound::Component::constParams)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::SelectorParameter>()
{
   return members(
      member("component", &base::musicDevice::description::sound::SelectorParameter::component),
      member("name", &base::musicDevice::description::sound::SelectorParameter::name)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::OneOfComponents>()
{
   return members(
      member("name", &base::musicDevice::description::sound::OneOfComponents::name),
      member("selectorParam", &base::musicDevice::description::sound::OneOfComponents::selectorParam),
      member("oneOfComponents", &base::musicDevice::description::sound::OneOfComponents::oneOfComponents),
      member("role", &base::musicDevice::description::sound::OneOfComponents::role)
   );
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::sound::Component>(int i) noexcept
{
   return "Component";
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::sound::OneOfComponents>(int i) noexcept
{
   return "OneOfComponents";
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::sound::MidiCCAndValue>(int i) noexcept
{
   return "MidiCCAndValue";
}

template <>
inline auto getClassNameOrIndex<base::musicDevice::description::sound::MidiSysexMsg>(int i) noexcept
{
   return "MidiSysexMsg";
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ValueRangeInt>()
{
   return members(
      member("from", &base::musicDevice::description::sound::ValueRangeInt::from),
      member("to", &base::musicDevice::description::sound::ValueRangeInt::to)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterSourceRangeMidi>()
{
   return members(
      member("name", &base::musicDevice::description::sound::ParameterSourceRangeMidi::name),
      member("range", &base::musicDevice::description::sound::ParameterSourceRangeMidi::range),
      member("role", &base::musicDevice::description::sound::ParameterSourceRangeMidi::role)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterSourceMidi>()
{
   return members(
      member("id", &base::musicDevice::description::sound::ParameterSourceMidi::id),
      member("sourceValueRange", &base::musicDevice::description::sound::ParameterSourceMidi::sourceValueRange),
      member("sourceRangesFrom", &base::musicDevice::description::sound::ParameterSourceMidi::sourceRangesFrom),
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
      member("defaultValue", &base::musicDevice::description::sound::Parameter::defaultValue),
      member("description", &base::musicDevice::description::sound::Parameter::description),
      member("source", &base::musicDevice::description::sound::Parameter::source)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Global>()
{
   return members(
      member("midiChannel", &base::musicDevice::description::sound::Global::midiChannel),
      member("parameterDumpRequest", &base::musicDevice::description::sound::Global::parameterDumpRequest),
      member("parameterDumpAnswer", &base::musicDevice::description::sound::Global::parameterDumpAnswer),
      member("components", &base::musicDevice::description::sound::Global::components),
      member("parameters", &base::musicDevice::description::sound::Global::parameters),
      member("presets", &base::musicDevice::description::sound::Global::presets)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Engine>()
{
   return members(
      member("name", &base::musicDevice::description::sound::Engine::name),
      member("from", &base::musicDevice::description::sound::Engine::from),
      member("noteSettings", &base::musicDevice::description::sound::Engine::noteSettings),
      member("components", &base::musicDevice::description::sound::Engine::components),
      member("parameters", &base::musicDevice::description::sound::Engine::parameters),
      member("presets", &base::musicDevice::description::sound::Engine::presets),
      member("parameterDumpRequest", &base::musicDevice::description::sound::Engine::parameterDumpRequest),
      member("parameterDumpAnswer", &base::musicDevice::description::sound::Engine::parameterDumpAnswer)
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
inline auto registerMembers<base::musicDevice::description::sound::MidiSysexMsg>()
{
   return members(
      member("sysexDescriptors", &base::musicDevice::description::sound::MidiSysexMsg::sysexDescriptors)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::ParameterDumpRequest>()
{
   return members(
      member("effect", &base::musicDevice::description::sound::ParameterDumpRequest::effect),
      member("message", &base::musicDevice::description::sound::ParameterDumpRequest::message)
   );
}


template <>
inline auto registerMembers<base::musicDevice::description::sound::Presets>()
{
   return members(
      member("numberOfPresets", &base::musicDevice::description::sound::Presets::numberOfPresets)
   );
}

template <>
inline auto registerMembers<base::musicDevice::description::sound::Section>()
{
   return members(
      member("defaultInstrumentType", &base::musicDevice::description::sound::Section::defaultInstrumentType),
      member("maxPitchBendRange", &base::musicDevice::description::sound::Section::maxPitchBendRange),
      member("voices", &base::musicDevice::description::sound::Section::voices),
      member("global", &base::musicDevice::description::sound::Section::global),
      member("engines", &base::musicDevice::description::sound::Section::engines),
      member("engineTemplates", &base::musicDevice::description::sound::Section::engineTemplates),
      member("sourceRanges", &base::musicDevice::description::sound::Section::sourceRanges)
   );
}

} // namespace meta


#endif