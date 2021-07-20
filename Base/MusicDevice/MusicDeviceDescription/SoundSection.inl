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
// base::musicDevice::description::sound::Component::Type
// --------------------------------------------------------
inline std::string
base::musicDevice::description::sound::Component::type2String(Type type)
{
   switch (type)
   {
      case Type::Filter: return "Filter";
      case Type::Envelope: return "Envelope";
      case Type::Arpeggiator: return "Arpeggiator";
   }
   return "Unknown";
}

inline base::musicDevice::description::sound::Component::Type
base::musicDevice::description::sound::Component::typeFromString(
    const std::string& str)
{
   if (str == "Filter")
      return Type::Filter;
   else if (str == "Envelope")
      return Type::Envelope;
   else if (str == "Arpeggiator")
      return Type::Arpeggiator;
   return Type::Unknown;
}

template <>
inline void to_json<base::musicDevice::description::sound::Component::Type>(
    nlohmann::json& j,
    const base::musicDevice::description::sound::Component::Type& obj)
{
   j = base::musicDevice::description::sound::Component::type2String(obj);
}

template <>
inline void from_json<base::musicDevice::description::sound::Component::Type>(
    const nlohmann::json& j,
    base::musicDevice::description::sound::Component::Type& obj)
{
   obj = base::musicDevice::description::sound::Component::typeFromString(
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

#endif