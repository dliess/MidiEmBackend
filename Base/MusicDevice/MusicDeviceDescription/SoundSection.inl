#ifndef SOUND_SECTION_INL_H
#define SOUND_SECTION_INL_H

#include "JsonCast.h"
#include "SoundSection.h"
#include "VectorIndexInRange.h"

// --------------------------------------------------------
// base::musicDevice::description::sound::Section::DefaultInstrumentType
// --------------------------------------------------------
inline std::string
base::musicDevice::description::sound::Section::defaultInstrumentType2String(
    DefaultInstrumentType type)
{
   switch (type)
   {
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::DrumKit:
         return "DrumKit";
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::InstrumentPerVoice:
         return "InstrumentPerVoice";
      case base::musicDevice::description::sound::Section::
          DefaultInstrumentType::OnePolyphonicInstrument:
         return "OnePolyphonicInstrument";
      default: return "unknown";
   }
}
inline base::musicDevice::description::sound::Section::DefaultInstrumentType
base::musicDevice::description::sound::Section::defaultInstrumentTypeFromString(
    const std::string& str)
{
   if (str == "DrumKit")
      return base::musicDevice::description::sound::Section::
          DefaultInstrumentType::DrumKit;
   else if (str == "InstrumentPerVoice")
      return base::musicDevice::description::sound::Section::
          DefaultInstrumentType::InstrumentPerVoice;
   else if (str == "OnePolyphonicInstrument")
      return base::musicDevice::description::sound::Section::
          DefaultInstrumentType::OnePolyphonicInstrument;
   return base::musicDevice::description::sound::Section::
       DefaultInstrumentType::InstrumentPerVoice;
}
template <>
inline void
to_json<base::musicDevice::description::sound::Section::DefaultInstrumentType>(
    nlohmann::json& j,
    const base::musicDevice::description::sound::Section::DefaultInstrumentType&
        obj)
{
   j = base::musicDevice::description::sound::Section::
       defaultInstrumentType2String(obj);
}
template <>
inline void from_json<
    base::musicDevice::description::sound::Section::DefaultInstrumentType>(
    const nlohmann::json& j,
    base::musicDevice::description::sound::Section::DefaultInstrumentType& obj)
{
   obj = base::musicDevice::description::sound::Section::
       defaultInstrumentTypeFromString(j.get<std::string>());
}

// --------------------------------------------------------
// base::musicDevice::description::sound::Component::Role
// --------------------------------------------------------
inline std::string
base::musicDevice::description::sound::Component::role2String(Role role)
{
   switch (role)
   {
      case Role::NoteTrigger: return "NoteTrigger";
      case Role::Amp: return "Amp";
      case Role::Filter: return "Filter";
      case Role::Envelope: return "Envelope";
      case Role::LFO: return "LFO";
      case Role::Arpeggiator: return "Arpeggiator";
   }
   return "Unknown";
}

inline base::musicDevice::description::sound::Component::Role
base::musicDevice::description::sound::Component::roleFromString(
    const std::string& roleStr)
{
   if (roleStr == "NoteTrigger")
      return Role::NoteTrigger;
   else if (roleStr == "Amp")
      return Role::Amp;
   else if (roleStr == "Filter")
      return Role::Filter;
   else if (roleStr == "Envelope")
      return Role::Envelope;
   else if (roleStr == "LFO")
      return Role::LFO;
   else if (roleStr == "Arpeggiator")
      return Role::Arpeggiator;
   return Role::Unknown;
}

template <>
inline void to_json<base::musicDevice::description::sound::Component::Role>(
    nlohmann::json& j,
    const base::musicDevice::description::sound::Component::Role& obj)
{
   j = base::musicDevice::description::sound::Component::role2String(obj);
}

template <>
inline void from_json<base::musicDevice::description::sound::Component::Role>(
    const nlohmann::json& j,
    base::musicDevice::description::sound::Component::Role& obj)
{
   obj = base::musicDevice::description::sound::Component::roleFromString(
       j.get<std::string>());
}

// --------------------------------------------------------
// SoundDeviceParameter::Type
// --------------------------------------------------------
template <>
inline void to_json<base::musicDevice::description::sound::Parameter::Type>(
    nlohmann::json& j,
    const base::musicDevice::description::sound::Parameter::Type& obj)
{
   j = base::musicDevice::description::sound::Parameter::type2String(obj);
}

template <>
inline void from_json<base::musicDevice::description::sound::Parameter::Type>(
    const nlohmann::json& j,
    base::musicDevice::description::sound::Parameter::Type& obj)
{
   obj = base::musicDevice::description::sound::Parameter::typeFromString(
       j.get<std::string>());
}

inline std::string
base::musicDevice::description::sound::Parameter::type2String(
    base::musicDevice::description::sound::Parameter::Type type)
{
   switch (type)
   {
      case base::musicDevice::description::sound::Parameter::Type::Continous:
         return "continous";
      case base::musicDevice::description::sound::Parameter::Type::
          ContinousBipolar:
         return "continous-bipolar";
      case base::musicDevice::description::sound::Parameter::Type::List:
         return "list";
      default: return "unknown";
   }
}

inline base::musicDevice::description::sound::Parameter::Type
base::musicDevice::description::sound::Parameter::typeFromString(
    const std::string& str)
{
   if (str == "continous")
      return base::musicDevice::description::sound::Parameter::Type::Continous;
   else if (str == "continous-bipolar")
      return base::musicDevice::description::sound::Parameter::Type::
          ContinousBipolar;
   else if (str == "list")
      return base::musicDevice::description::sound::Parameter::Type::List;
   return base::musicDevice::description::sound::Parameter::Type::Continous;
}

// --------------------------------------------------------
// SoundDeviceParameter::Role
// --------------------------------------------------------
template <>
inline void to_json<base::musicDevice::description::sound::Parameter::Role>(
    nlohmann::json& j,
    const base::musicDevice::description::sound::Parameter::Role& obj)
{
   j = base::musicDevice::description::sound::Parameter::role2String(obj);
}

template <>
inline void from_json<base::musicDevice::description::sound::Parameter::Role>(
    const nlohmann::json& j,
    base::musicDevice::description::sound::Parameter::Role& obj)
{
   obj = base::musicDevice::description::sound::Parameter::roleFromString(
       j.get<std::string>());
}

inline std::string
base::musicDevice::description::sound::Parameter::role2String(Role role)
{
   switch (role)
   {
      case Role::Unknown: return "Unknown";
      case Role::FilterCutoff: return "FilterCutoff";
      case Role::FilterType: return "FilterType";
      case Role::FilterResonance: return "FilterResonance";
      case Role::Pitch: return "Pitch";
      case Role::PitchFinetune: return "PitchFinetune";
      case Role::OscShape: return "OscShape";
      case Role::Volume: return "Volume";
      case Role::Pan: return "Pan";
      case Role::Mute: return "Mute";
      case Role::Attack: return "Attack";
      case Role::Decay: return "Decay";
      case Role::Sustain: return "Sustain";
      case Role::Release: return "Release";
      case Role::SampleStart: return "SampleStart";
      case Role::SampleEnd: return "SampleEnd";
      case Role::SampleLength: return "SampleLength";
      case Role::SampleLoop: return "SampleLoop";
      case Role::SampleReverse: return "SampleReverse";
      case Role::SampleRate: return "SampleRate";
      case Role::DelaySend: return "DelaySend";
      case Role::ReverbSend: return "ReverbSend";
      case Role::LFOSpeed: return "LFOSpeed";
      case Role::LFOMultiplier: return "LFOMultiplier";
      case Role::LFOWaveform: return "LFOWaveform";
      case Role::LFODestination: return "LFODestination";
      case Role::LFODepth: return "LFODepth";
      case Role::TrigChance: return "TrigChance";
      default: return "Unknown";
   }
}

inline base::musicDevice::description::sound::Parameter::Role
base::musicDevice::description::sound::Parameter::roleFromString(
    const std::string& roleStr)
{
   if (roleStr == "FilterCutoff")
      return Role::FilterCutoff;
   else if (roleStr == "FilterType")
      return Role::FilterType;
   else if (roleStr == "FilterResonance")
      return Role::FilterResonance;
   else if (roleStr == "Pitch")
      return Role::Pitch;
   else if (roleStr == "PitchFinetune")
      return Role::PitchFinetune;
   else if (roleStr == "OscShape")
      return Role::OscShape;
   else if (roleStr == "Volume")
      return Role::Volume;
   else if (roleStr == "Pan")
      return Role::Pan;
   else if (roleStr == "Mute")
      return Role::Mute;
   else if (roleStr == "Attack")
      return Role::Attack;
   else if (roleStr == "Decay")
      return Role::Decay;
   else if (roleStr == "Sustain")
      return Role::Sustain;
   else if (roleStr == "Release")
      return Role::Release;
   else if (roleStr == "SampleStart")
      return Role::SampleStart;
   else if (roleStr == "SampleEnd")
      return Role::SampleEnd;
   else if (roleStr == "SampleLength")
      return Role::SampleLength;
   else if (roleStr == "SampleLoop")
      return Role::SampleLoop;
   else if (roleStr == "SampleReverse")
      return Role::SampleReverse;
   else if (roleStr == "SampleRate")
      return Role::SampleRate;
   else if (roleStr == "DelaySend")
      return Role::DelaySend;
   else if (roleStr == "ReverbSend")
      return Role::ReverbSend;
   else if (roleStr == "LFOSpeed")
      return Role::LFOSpeed;
   else if (roleStr == "LFOMultiplier")
      return Role::LFOMultiplier;
   else if (roleStr == "LFOWaveform")
      return Role::LFOWaveform;
   else if (roleStr == "LFODestination")
      return Role::LFODestination;
   else if (roleStr == "LFODepth")
      return Role::LFODepth;
   else if (roleStr == "TrigChance")
      return Role::TrigChance;
   return Role::Unknown;
}

// --------------------------------------------------------
// base::musicDevice::description::sound::Section
// --------------------------------------------------------

namespace base::musicDevice::description::sound
{
inline const Parameter& Section::parameterDescr(
    const ParameterId& parameterId) const noexcept
{
   assert(parameterId.engineId >= 0 && parameterId.engineId < engines.size());
   assert(parameterId.parameterId >= 0 &&
          parameterId.parameterId <
              engines[parameterId.engineId].parameters.size());
   return engines[parameterId.engineId].parameters[parameterId.parameterId];
}

}   // namespace base::musicDevice::description::sound

inline const base::musicDevice::description::sound::Parameter&
base::musicDevice::description::sound::Section::parameterDescr(
    int voiceId, int parameterId) const noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == voiceId)
   {
      assert(global);
      assert(parameterId >= 0 && parameterId < global->parameters.size());
      return global->parameters[parameterId];
   }
   else
   {
      assert(voiceId >= 0 && voiceId < voices.size());
      assert(parameterId >= 0 &&
             parameterId < engines[voices[voiceId].engineId].parameters.size());
      return engines[voices[voiceId].engineId].parameters[parameterId];
   }
}

inline const base::musicDevice::description::sound::EngineBase*
base::musicDevice::description::sound::Section::engineBase(
    int voiceId) const noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == voiceId)
   {
      assert(global);
      return &global.value();
   }
   else
   {
      assert(voiceId >= 0 && voiceId < voices.size());
      return &engines[voices[voiceId].engineId];
   }
}

inline int base::musicDevice::description::sound::Section::getMidiChannel(
    int voiceId) const noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == voiceId)
   {
      assert(global);
      return global->midiChannel;
   }
   else
   {
      assert(voiceId >= 0 && voiceId < voices.size());
      return voices[voiceId].midiChannel;
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachParameterDescr(
    T&& cb) noexcept
{
   if (global)
   {
      for (int paramIdx = 0; paramIdx < global->parameters.size(); ++paramIdx)
      {
         cb({GlobalSectionId, paramIdx}, global->parameters[paramIdx]);
      }
   }
   for (int engineIdx = 0; engineIdx < engines.size(); ++engineIdx)
   {
      for (int paramIdx = 0; paramIdx < engines[engineIdx].parameters.size();
           ++paramIdx)
      {
         cb({engineIdx, paramIdx}, engines[engineIdx].parameters[paramIdx]);
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachParameterDescr(
    T&& cb) const noexcept
{
   if (global)
   {
      for (int paramIdx = 0; paramIdx < global->parameters.size(); ++paramIdx)
      {
         cb({GlobalSectionId, paramIdx}, global->parameters[paramIdx]);
      }
   }
   for (int engineIdx = 0; engineIdx < engines.size(); ++engineIdx)
   {
      for (int paramIdx = 0; paramIdx < engines[engineIdx].parameters.size();
           ++paramIdx)
      {
         cb({engineIdx, paramIdx}, engines[engineIdx].parameters[paramIdx]);
      }
   }
}

inline bool base::musicDevice::description::sound::Section::hasParameters()
    const noexcept
{
   int cnt = 0;
   forEachParameterDescr(
       [&cnt](
           const base::musicDevice::description::sound::ParameterId& paramId,
           const base::musicDevice::description::sound::Parameter& parameter) {
          cnt += 1;
       });
   return cnt;
}

inline int base::musicDevice::description::sound::Section::voice2EngineIdx(
    int voiceIdx) const noexcept
{
   if (GlobalSectionId == voiceIdx)
   {
      return GlobalSectionId;
   }
   else
   {
      assert(util::vector_index_in_range(voiceIdx, voices));
      return voices[voiceIdx].engineId;
   }
}

template <typename T>
int base::musicDevice::description::sound::Section::linSearchByName(
    const std::vector<T>& vector, const std::string& name) noexcept
{
   for (int i = 0; i < vector.size(); ++i)
   {
      if (vector[i].name == name)
         return i;
   }
   return -1;
}

inline float base::musicDevice::description::sound::Section::getInitialValueFor(
    int voiceId, int parameterId) const noexcept
{
   const float val = _getInitialValueFor(voiceId, parameterId);
   if (val >= 1.0)
   {
      const auto& paramDescr = parameterDescr(voiceId, parameterId);
      const int idx = int(val) - 1;
      return paramDescr.getListValueByIndex(idx);
   }
   return val;
}

inline float
base::musicDevice::description::sound::Section::_getInitialValueFor(
    int voiceId, int parameterId) const noexcept
{
   const auto& paramDescr = parameterDescr(voiceId, parameterId);
   if (paramDescr.defaultValue.has_value())
   {
      return *paramDescr.defaultValue;
   }
   if (!paramDescr.role.has_value())
   {
      return 0.0;
   }
   switch (*paramDescr.role)
   {   // Use value range from 0.0 .. 0.99 for continous values, 1.0 2.0 3.0 for
       // list items
      case Parameter::Role::Unknown: return 0.0;
      case Parameter::Role::FilterCutoff: return 0.5;
      case Parameter::Role::FilterType: return 0.0;
      case Parameter::Role::FilterResonance: return 0.0;
      case Parameter::Role::Pitch: return 0.5;
      case Parameter::Role::PitchFinetune: return 0.5;
      case Parameter::Role::OscShape: return 1;
      case Parameter::Role::Volume: return 0.8;
      case Parameter::Role::Pan: return 0.5;
      case Parameter::Role::Mute: return 0.0;
      case Parameter::Role::Attack: return 0.0;
      case Parameter::Role::Decay: return 0.3;
      case Parameter::Role::Sustain: return 0.99;
      case Parameter::Role::Release: return 0.3;
      case Parameter::Role::SampleStart: return 0.0;
      case Parameter::Role::SampleEnd: return 0.99;
      case Parameter::Role::SampleLength: return 0.99;
      case Parameter::Role::SampleLoop: return 0.0;
      case Parameter::Role::SampleReverse: return 0.0;
      case Parameter::Role::SampleRate: return 0.99;
      case Parameter::Role::DelaySend: return 0.0;
      case Parameter::Role::ReverbSend: return 0.0;
      case Parameter::Role::LFOSpeed: return 0.0;
      case Parameter::Role::LFOMultiplier: return 0.0;
      case Parameter::Role::LFOWaveform: return 0.0;
      case Parameter::Role::LFODestination: return 0.0;
      case Parameter::Role::LFODepth: return 0.0;
      case Parameter::Role::TrigChance: return 0.99;
   }
   return 0.0;
}

inline float
base::musicDevice::description::sound::Parameter::getListValueByIndex(
    int idx) const noexcept
{
   assert(source.sourceRanges.has_value());
   if (source.sourceRanges->at(idx).range.has_value())
   {
      const auto& range = *source.sourceRanges->at(idx).range;
      return (range.from + range.to) / 2.0;
   }
   else
   {
      const float arcLen = 1.0 / source.sourceRanges->size();
      return (arcLen * idx + arcLen * (idx + 1)) / 2.0;
   }
}

#endif