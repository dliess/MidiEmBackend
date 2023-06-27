#ifndef SOUND_SECTION_INL_H
#define SOUND_SECTION_INL_H

#include <spdlog/spdlog.h>

#include "JsonCast.h"
#include "Overload.h"
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

namespace base::musicDevice::description::sound
{
inline void to_json(nlohmann::json& j,
                    const Section::DefaultInstrumentType& obj)
{
   j = Section::defaultInstrumentType2String(obj);
}
inline void from_json(const nlohmann::json& j,
                      Section::DefaultInstrumentType& obj)
{
   obj = Section::defaultInstrumentTypeFromString(j.get<std::string>());
}
}   // namespace base::musicDevice::description::sound
// --------------------------------------------------------
// base::musicDevice::description::sound::Component::Role
// --------------------------------------------------------
inline std::string
base::musicDevice::description::sound::Component::role2String(Role role)
{
   return ~role;
}

inline base::musicDevice::description::sound::Component::Role
base::musicDevice::description::sound::Component::roleFromString(
    const std::string& roleStr)
{
   return create_ComponentRole(roleStr);
}

namespace base::musicDevice::description::sound
{
inline void to_json(nlohmann::json& j, const Component::Role& obj)
{
   j = Component::role2String(obj);
}

inline void from_json(const nlohmann::json& j, Component::Role& obj)
{
   obj = Component::roleFromString(j.get<std::string>());
}

// --------------------------------------------------------
// SoundDeviceParameter::Type
// --------------------------------------------------------
inline void to_json(nlohmann::json& j, const Parameter::Type& obj)
{
   j = Parameter::type2String(obj);
}

inline void from_json(const nlohmann::json& j, Parameter::Type& obj)
{
   obj = Parameter::typeFromString(j.get<std::string>());
}
}   // namespace base::musicDevice::description::sound

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
// ParameterSourceRange::Role
// --------------------------------------------------------
namespace base::musicDevice::description::sound
{
inline void to_json(nlohmann::json& j,
                    const ParameterSourceRangeBase::Role& obj)
{
   j = ParameterSourceRangeBase::role2String(obj);
}

inline void from_json(const nlohmann::json& j,
                      ParameterSourceRangeBase::Role& obj)
{
   obj = ParameterSourceRangeBase::roleFromString(j.get<std::string>());
}
}   // namespace base::musicDevice::description::sound

inline std::string
base::musicDevice::description::sound::ParameterSourceRangeBase::role2String(
    Role role)
{
   return ~role;
}

inline base::musicDevice::description::sound::ParameterSourceRangeBase::Role
base::musicDevice::description::sound::ParameterSourceRangeBase::roleFromString(
    const std::string& roleStr)
{
   return create_ParameterSourceRangeBaseRoles(roleStr);
}

// --------------------------------------------------------
// SoundDeviceParameter::Role
// --------------------------------------------------------
namespace base::musicDevice::description::sound
{
inline void to_json(nlohmann::json& j, const Parameter::Role& obj)
{
   j = Parameter::role2String(obj);
}

inline void from_json(const nlohmann::json& j, Parameter::Role& obj)
{
   obj = Parameter::roleFromString(j.get<std::string>());
}

inline std::string Parameter::role2String(Role role) { return ~role; }

inline Parameter::Role Parameter::roleFromString(const std::string& roleStr)
{
   return create_ParameterRole(roleStr);
}

// --------------------------------------------------------
// ParameterDumpRequest::Effect
// --------------------------------------------------------
inline void to_json(nlohmann::json& j, const ParameterDumpRequest::Effect& obj)
{
   j = ~obj;
}

inline void from_json(const nlohmann::json& j,
                      ParameterDumpRequest::Effect& obj)
{
   obj = create_ParameterDumpRequestEffect(j.get<std::string>());
}
}   // namespace base::musicDevice::description::sound

// --------------------------------------------------------
// base::musicDevice::description::sound::Section
// --------------------------------------------------------

namespace base::musicDevice::description::sound
{
inline const Parameter& Section::parameterDescription(
    const ParameterId& parameterId) const noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId ==
       parameterId.engineId)
   {
      assert(global);
      assert(parameterId.parameterId >= 0 &&
             parameterId.parameterId < global->parameters.size());
      return global->parameters[parameterId.parameterId];
   }
   else
   {
      assert(parameterId.engineId >= 0 &&
             parameterId.engineId < engines.size());
      assert(parameterId.parameterId >= 0 &&
             parameterId.parameterId <
                 engines[parameterId.engineId].parameters.size());
      return engines[parameterId.engineId].parameters[parameterId.parameterId];
   }
}

}   // namespace base::musicDevice::description::sound

inline const base::musicDevice::description::sound::Parameter&
base::musicDevice::description::sound::Section::parameterDescription(
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

inline const base::musicDevice::description::sound::MidiChannelHolder*
base::musicDevice::description::sound::Section::midiChannel(
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
      return &voices[voiceId];
   }
}

template <typename Cb>
void base::musicDevice::description::sound::Section::forEachEngineBase(Cb&& cb)
{
   if (global)
   {
      cb(GlobalSectionId, *global);
   }
   for (int i = 0; i < engines.size(); ++i) { cb(i, engines[i]); }
}

template <typename Cb>
void base::musicDevice::description::sound::Section::forEachEngineBase(
    Cb&& cb) const
{
   if (global)
   {
      cb(GlobalSectionId, *global);
   }
   for (int i = 0; i < engines.size(); ++i) { cb(i, engines[i]); }
}

template <typename Cb>
void base::musicDevice::description::sound::Section::forEachVoiceOfEngine(
    int engineIdx, Cb&& cb) const
{
   if (engineIdx == GlobalSectionId)
   {
      cb(GlobalSectionId, GlobalSectionId);
   }
   else
   {
      for (int i = 0; i < voices.size(); ++i)
      {
         if (voices[i].engineId == engineIdx)
         {
            cb(i, engineIdx);
         }
      }
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

inline std::optional<int>
base::musicDevice::description::sound::Section::getVoiceIdx(
    int midiChannelNr) const noexcept
{
   if (global)
   {
      if (global->midiChannel == midiChannelNr)
      {
         return base::musicDevice::description::sound::GlobalSectionId;
      }
   }
   for (int i = 0; i < voices.size(); ++i)
   {
      if (voices[i].midiChannel == midiChannelNr)
      {
         return i;
      }
   }
   return std::nullopt;
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
template <typename T>
void base::musicDevice::description::sound::Section::forEachParameterDescr(
    int engineIdx, T&& cb) noexcept
{
   if (engineIdx == GlobalSectionId)
   {
      if (global)
      {
         for (int paramIdx = 0; paramIdx < global->parameters.size();
              ++paramIdx)
         {
            cb(paramIdx, global->parameters[paramIdx]);
         }
      }
   }
   else
   {
      for (int paramIdx = 0; paramIdx < engines[engineIdx].parameters.size();
           ++paramIdx)
      {
         cb(paramIdx, engines[engineIdx].parameters[paramIdx]);
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachParameterDescr(
    int engineIdx, T&& cb) const noexcept
{
   if (engineIdx == GlobalSectionId)
   {
      if (global)
      {
         for (int paramIdx = 0; paramIdx < global->parameters.size();
              ++paramIdx)
         {
            cb(paramIdx, global->parameters[paramIdx]);
         }
      }
   }
   else
   {
      for (int paramIdx = 0; paramIdx < engines[engineIdx].parameters.size();
           ++paramIdx)
      {
         cb(paramIdx, engines[engineIdx].parameters[paramIdx]);
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachComponentDescr(
    T&& cb) noexcept
{
   if (global && global->components)
   {
      for (int componentIdx = 0; componentIdx < global->components->size();
           ++componentIdx)
      {
         cb(GlobalSectionId, componentIdx,
            global->components->operator[](componentIdx));
      }
   }
   for (int engineIdx = 0; engineIdx < engines.size(); ++engineIdx)
   {
      for (int componentIdx = 0;
           componentIdx < engines[engineIdx].components->size(); ++componentIdx)
      {
         cb(engineIdx, componentIdx,
            engines[engineIdx].components->operator[](componentIdx));
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachComponentDescr(
    T&& cb) const noexcept
{
   if (global && global->components)
   {
      for (int componentIdx = 0; componentIdx < global->components->size();
           ++componentIdx)
      {
         cb(GlobalSectionId, componentIdx,
            global->components->operator[](componentIdx));
      }
   }
   for (int engineIdx = 0; engineIdx < engines.size(); ++engineIdx)
   {
      for (int componentIdx = 0;
           componentIdx < engines[engineIdx].components->size(); ++componentIdx)
      {
         cb(engineIdx, componentIdx,
            engines[engineIdx].components->operator[](componentIdx));
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachComponentDescr(
    int engineIdx, T&& cb) noexcept
{
   if (engineIdx == GlobalSectionId)
   {
      if (global && global->components)
      {
         for (int componentIdx = 0; componentIdx < global->components->size();
              ++componentIdx)
         {
            cb(componentIdx, global->components->operator[](componentIdx));
         }
      }
   }
   else
   {
      assert(engineIdx >= 0 && engineIdx < engines.size());
      for (int componentIdx = 0;
           componentIdx < engines[engineIdx].components->size(); ++componentIdx)
      {
         cb(componentIdx,
            engines[engineIdx].components->operator[](componentIdx));
      }
   }
}

template <typename T>
void base::musicDevice::description::sound::Section::forEachComponentDescr(
    int engineIdx, T&& cb) const noexcept
{
   if (engineIdx == GlobalSectionId)
   {
      if (global && global->components)
      {
         for (int componentIdx = 0; componentIdx < global->components->size();
              ++componentIdx)
         {
            cb(componentIdx, global->components->operator[](componentIdx));
         }
      }
   }
   else
   {
      assert(engineIdx >= 0 && engineIdx < engines.size());
      for (int componentIdx = 0;
           componentIdx < engines[engineIdx].components->size(); ++componentIdx)
      {
         cb(componentIdx,
            engines[engineIdx].components->operator[](componentIdx));
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
   const auto& paramDescr = parameterDescription(voiceId, parameterId);
   return mpark::visit(
       util::overload{
           [this, paramDescr](const int& val) -> float { return val; },
           [this, paramDescr](const double& val) -> float { return val; },
           [this,
            paramDescr](const base::musicDevice::description::sound::
                            ParameterSourceRangeBase::Role& role) -> float {
              if (paramDescr.type != Parameter::Type::List)
                 return 0;
              const auto retVal = paramDescr.getListIndexByListRole(role);
              if (retVal)
              {
                 return *retVal;
              }
              return 0;
           }},
       _getInitialValueFor(voiceId, parameterId));
}

inline mpark::variant<
    int, double,
    base::musicDevice::description::sound::ParameterSourceRangeBase::Role>
base::musicDevice::description::sound::Section::_getInitialValueFor(
    int voiceId, int parameterId) const noexcept
{
   const auto& paramDescr = parameterDescription(voiceId, parameterId);
   if (paramDescr.defaultValue.has_value())
   {
      return *paramDescr.defaultValue;
   }
   if (paramDescr.type == Parameter::Type::ContinousBipolar)
   {
      return 0.5;
   }
   if (!paramDescr.role.has_value())
   {
      return 0.0;
   }
   switch (*paramDescr.role)
   {   // Use value range from 0.0 .. 0.99 for continous values, 1.0 2.0 3.0 for
       // list items
      case Parameter::Role::Unknown: return 0.0;
      case Parameter::Role::TrackVolume: return 0.8;
      case Parameter::Role::TrigChance: return 0.99;
      case Parameter::Role::Swing: return 0.0;
      case Parameter::Role::Mute: return ParameterSourceRangeBase::Role::Off;
      case Parameter::Role::Solo: return ParameterSourceRangeBase::Role::Off;
      case Parameter::Role::KeyPrioMode: return 0.0;
      case Parameter::Role::Pitch: return 0.5;
      case Parameter::Role::PitchFinetune: return 0.5;
      case Parameter::Role::Detune: return 0.5;
      case Parameter::Role::OSCWaveform: return 1;
      case Parameter::Role::OSCShape: return 1;
      case Parameter::Role::OSCGlide: return 0.0;
      case Parameter::Role::OSCGlideOnOff:
         return ParameterSourceRangeBase::Role::Off;
      case Parameter::Role::OSCGlideType: return 1;
      case Parameter::Role::OSCKeyboardTracking:
         return ParameterSourceRangeBase::Role::On;
      case Parameter::Role::OSCSync: return 0.0;
      case Parameter::Role::OSCSlop: return 0.0;
      case Parameter::Role::OSCMix: return 0.0;
      case Parameter::Role::OSCNoise: return 0.0;
      case Parameter::Role::OSCNoteSyncOnOff:
         return ParameterSourceRangeBase::Role::On;
      case Parameter::Role::OSC1Volume: return 0.8;
      case Parameter::Role::OSC2Volume: return 0.0;
      case Parameter::Role::SubOSCLevel: return 0.0;
      case Parameter::Role::NoiseLevel: return 0.0;
      case Parameter::Role::NoiseDecay: return 0.5;
      case Parameter::Role::UnisonMode: return 0.0;
      case Parameter::Role::UnisonOnOff: return 0.0;
      case Parameter::Role::FMAmount: return 0.0;
      case Parameter::Role::InitialVolume: return 0.0;
      case Parameter::Role::Volume: return 0.4;
      case Parameter::Role::Pan: return 0.5;
      case Parameter::Role::DelaySend: return 0.0;
      case Parameter::Role::ReverbSend: return 0.0;
      case Parameter::Role::FeedbackVolume: return 0.0;
      case Parameter::Role::FeedbackGain: return 0.0;
      case Parameter::Role::FilterCutoff: return 0.99;
      case Parameter::Role::FilterBase: return 0.0;
      case Parameter::Role::FilterWidth: return 0.99;
      case Parameter::Role::FilterType: return 1;
      case Parameter::Role::FilterResonance: return 0.0;
      case Parameter::Role::FilterSlope: return 0.0;
      case Parameter::Role::FilterKeyAmount: return 0.0;
      case Parameter::Role::FilterAudioModuation: return 0.0;
      case Parameter::Role::LPFilterCutoff: return 0.99;
      case Parameter::Role::LPFilterResonance: return 0.0;
      case Parameter::Role::LPFilterSlope: return 0.0;
      case Parameter::Role::LPFilterKeyAmount: return 0.0;
      case Parameter::Role::LPFilterAudioModuation: return 0.0;
      case Parameter::Role::HPFilterCutoff: return 0.0;
      case Parameter::Role::HPFilterResonance: return 0.0;
      case Parameter::Role::HPFilterSlope: return 0.0;
      case Parameter::Role::HPFilterKeyAmount: return 0.0;
      case Parameter::Role::HPFilterAudioModuation: return 0.0;
      case Parameter::Role::LPHPFilterCutoff: return 0.5;
      case Parameter::Role::LPHPFilterResonance: return 0.0;
      case Parameter::Role::LPHPFilterSlope: return 0.0;
      case Parameter::Role::LPHPFilterKeyAmount: return 0.0;
      case Parameter::Role::LPHPFilterAudioModuation: return 0.0;
      case Parameter::Role::LPFilterSweep: return 0.0;
      case Parameter::Role::ComponentSelector: return 1;
      case Parameter::Role::AmpEnvAmount: return 0.99;
      case Parameter::Role::EnvAmount: return 0.5;
      case Parameter::Role::EnvVelAmount: return 0.0;
      case Parameter::Role::EnvRepeat: return 0.0;
      case Parameter::Role::EnvDelay: return 0.0;
      case Parameter::Role::EnvDestination: return 1;
      case Parameter::Role::Attack: return 0.0;
      case Parameter::Role::Decay: return 0.3;
      case Parameter::Role::Sustain: return 0.99;
      case Parameter::Role::Release: return 0.0;
      case Parameter::Role::Hold: return 0.5;
      case Parameter::Role::SweepTime: return 0.5;
      case Parameter::Role::SweepDepth: return 0.0;
      case Parameter::Role::LFOAmount: return 0.0;
      case Parameter::Role::LFOFrequency: return 0.0;
      case Parameter::Role::LFOMultiplier: return 0.0;
      case Parameter::Role::LFOWaveform: return 1;
      case Parameter::Role::LFODestination: return 1;
      case Parameter::Role::LFODepth: return 0.0;
      case Parameter::Role::LFOKeySync: return 0.0;
      case Parameter::Role::ArpOnOff: return 1;
      case Parameter::Role::ArpMode: return 0.0;
      case Parameter::Role::SequencerOnOff:
         return ParameterSourceRangeBase::Role::Off;
      case Parameter::Role::SequenceTrig: return 0.0;
      case Parameter::Role::SampleStart: return 0.0;
      case Parameter::Role::SampleEnd: return 0.99;
      case Parameter::Role::SampleLength: return 0.99;
      case Parameter::Role::SampleLoop: return 0.0;
      case Parameter::Role::SampleReverse: return 0.0;
      case Parameter::Role::SampleRate: return 0.5;
      case Parameter::Role::SampleBitReduction: return 0.0;
      case Parameter::Role::SampleSlot: return 1;
      case Parameter::Role::ModSource: return 1;
      case Parameter::Role::ModDestination: return 1;
      case Parameter::Role::ModAmount: return 0.5;
      case Parameter::Role::PitchBendRange: return 0.0;
      case Parameter::Role::BPM: return 0.0;
      case Parameter::Role::ClockDivide: return 0.0;
   }
   return 0.0;
}

inline int
base::musicDevice::description::sound::Parameter::getListIndexByValue(
    int value) const noexcept
{
   if (!source.midi->sourceRanges)
   {
      return 0;
   }
   const auto& ranges = *source.midi->sourceRanges;
   for (int idx = 0; idx < source.midi->sourceRanges->size(); ++idx)
   {
      if (ranges[idx].range.from <= value && value < ranges[idx].range.to)
      {
         return idx;
      }
   }
   return 0;
}

inline std::optional<float>
base::musicDevice::description::sound::Parameter::getListIndexByListRole(
    ParameterSourceRangeBase::Role role) const noexcept
{
   assert(source.midi->sourceRanges);
   for (int idx = 0; idx < source.midi->sourceRanges->size(); ++idx)
   {
      if (source.midi->sourceRanges->at(idx).role.has_value() &&
          role == *source.midi->sourceRanges->at(idx).role)
      {
         return float(idx);
      }
   }
   return std::nullopt;
}

inline int
base::musicDevice::description::sound::Parameter::getSourceResolution()
    const noexcept
{
   switch (type)
   {
      case base::musicDevice::description::sound::Parameter::Type::List:
      {
         if (source.midi->sourceRanges)
         {
            return static_cast<int>(source.midi->sourceRanges->size());
         }
         break;
      }
      case base::musicDevice::description::sound::Parameter::Type::Continous:
      case base::musicDevice::description::sound::Parameter::Type::
          ContinousBipolar:
      {
         if (source.midi)
         {
            if (source.midi->sourceValueRange)
            {
               return source.midi->sourceValueRange->to -
                      source.midi->sourceValueRange->from;
            }
            if (mpark::holds_alternative<midi::MidiMsgId<midi::NRPN>>(
                    source.midi->id) ||
                mpark::holds_alternative<
                    midi::MidiMsgId<midi::ControlChangeHighRes>>(
                    source.midi->id))
            {
               return 128 * 128;
            }
            else
            {
               return 128;
            }
         }
         break;
      }
   }
   return 0;
}

inline 
std::optional<ValueRangeEnd> base::musicDevice::description::sound::Parameter::getValueRange() const noexcept
{
   switch (type)
   {
      case base::musicDevice::description::sound::Parameter::Type::List:
      {
         if (source.midi->sourceRanges)
         {
            return ListRangeEnd(source.midi->sourceRanges->size());
         }
         break;
      }
      case base::musicDevice::description::sound::Parameter::Type::Continous:
      case base::musicDevice::description::sound::Parameter::Type::
          ContinousBipolar:
      {
         return FloatingPointRangeEnd(1.0f);
      }
   }
   return std::nullopt;
}

inline void base::musicDevice::description::sound::Section::
    autoFillSourceRangesForLists() noexcept
{
   forEachParameterDescr([this](const ParameterId& paramId,
                                Parameter& parameter) {
      if (parameter.type == Parameter::Type::List && parameter.source.midi &&
          parameter.source.midi->sourceValueRange &&
          !parameter.source.midi->sourceRanges &&
          !parameter.source.midi->sourceRangesFrom)
      {
         parameter.source.midi->sourceRanges.emplace();
         for (int i = parameter.source.midi->sourceValueRange->from;
              i <= parameter.source.midi->sourceValueRange->to; ++i)
         {
            ParameterSourceRangeMidi e;
            e.name       = std::to_string(i);
            e.range.from = i;
            e.range.to   = i;
            parameter.source.midi->sourceRanges->push_back(e);
         }
         parameter.source.midi->sourceValueRange = std::nullopt;
      }
   });
}

inline void base::musicDevice::description::sound::Section::
    fillParameterDumpOffsetCaches() noexcept
{
   forEachEngineBase([](int engineIdx, EngineBase& rEngineBase) {
      if (rEngineBase.parameterDumpRequest)
      {
         auto* pMsg = mpark::get_if<MidiSysexMsg>(
             &rEngineBase.parameterDumpRequest->message);
         if (pMsg)
         {
            int accumSize = 0;
            for (auto& fieldDescr : pMsg->sysexDescriptors)
            {
               accumSize +=
                   mpark::visit(util::overload{[accumSize](auto&& val) -> int {
                                   val.offset = accumSize;
                                   return val.sizeInSysex();
                                }},
                                fieldDescr);
            }
         }
      }
      if (rEngineBase.parameterDumpAnswer)
      {
         int accumSize = 0;
         for (auto& fieldDescr :
              rEngineBase.parameterDumpAnswer->sysexDescriptors)
         {
            accumSize +=
                mpark::visit(util::overload{[accumSize](auto&& val) -> int {
                                val.offset = accumSize;
                                return val.sizeInSysex();
                             }},
                             fieldDescr);
         }
      }
   });
}

inline bool base::musicDevice::description::sound::Section::isValidVoiceIdx(
    int voiceIdx) const noexcept
{
   return (GlobalSectionId == voiceIdx ||
           util::vector_index_in_range(voiceIdx, voices));
}

inline base::musicDevice::description::sound::Engine*
base::musicDevice::description::sound::Section::findParentEngineByName(
    const std::string& name) noexcept
{
   if (engineTemplates)
   {
      auto it = engineTemplates->find(name);
      if (it != engineTemplates->end())
      {
         return &(it->second);
      }
   }
   auto it =
       std::find_if(engines.begin(), engines.end(),
                    [&name](Engine& engine) { return engine.name == name; });
   if (it != engines.end())
   {
      return &(*it);
   }
   return nullptr;
}

inline void base::musicDevice::description::sound::Section::
    handleEngineInheritance() noexcept
{
   // we dont do it for global engines
   for (auto& engine : engines)
   {
      if (engine.from)
      {
         Engine* parentEngine = findParentEngineByName(*engine.from);
         assert(parentEngine);
         engine = inherit(*parentEngine, engine);
      }
   }
}

inline base::musicDevice::description::sound::Engine
base::musicDevice::description::sound::Section::inherit(
    const Engine& _parent, const Engine& child) noexcept
{
   Engine parent;
   if (_parent.from)
   {
      Engine* parentsParentEngine = findParentEngineByName(*_parent.from);
      assert(parentsParentEngine);
      parent = inherit(*parentsParentEngine, _parent);
   }
   else
   {
      parent = _parent;
   }
   Engine ret = child;
   if (!ret.noteSettings)
      ret.noteSettings = parent.noteSettings;
   if (!ret.components)
   {
      ret.components = parent.components;
   }
   else if (parent.components)
   {
      std::vector<ComponentVar> tmpComponents;
      for (auto& parentComponent : *parent.components)
      {
         auto it = std::find_if(ret.components->begin(), ret.components->end(),
                                [&parentComponent](const ComponentVar& c) {
                                   return VARIANT_GET(c, name) ==
                                          VARIANT_GET(parentComponent, name);
                                });
         if (it != std::end(*ret.components))
         {
            tmpComponents.push_back(compInherit(parentComponent, *it));
            ret.components->erase(it);
         }
         else
         {
            tmpComponents.push_back(parentComponent);
         }
      }
      tmpComponents.insert(tmpComponents.end(), ret.components->begin(),
                           ret.components->end());
      ret.components = tmpComponents;
   }

   std::vector<Parameter> tmpParameters;
   for (auto& parentParameter : parent.parameters)
   {
      auto it = std::find_if(ret.parameters.begin(), ret.parameters.end(),
                             [&parentParameter](const Parameter& p) {
                                return p.name == parentParameter.name &&
                                       p.component == parentParameter.component;
                             });
      if (it != std::end(ret.parameters))
      {
         tmpParameters.push_back(paramInherit(parentParameter, *it));
         ret.parameters.erase(it);
      }
      else
      {
         tmpParameters.push_back(parentParameter);
      }
   }
   tmpParameters.insert(tmpParameters.end(), ret.parameters.begin(),
                        ret.parameters.end());
   ret.parameters = tmpParameters;
   ret.from.reset();
   return ret;
}

inline base::musicDevice::description::sound::ComponentVar
base::musicDevice::description::sound::compInherit(
    const ComponentVar& parent, const ComponentVar& child) noexcept
{
   ComponentVar ret = child;
   mpark::visit(util::overload{
                    [](const Component&) {},
                    [&parent](OneOfComponents& ret) {
                       mpark::visit(util::overload{
                                        [](const Component&) {},
                                        [&ret](const OneOfComponents& parent) {
                                           if (parent.role && !ret.role)
                                              ret.role = parent.role;
                                           const auto tmp = ret.oneOfComponents;
                                           ret.oneOfComponents =
                                               parent.oneOfComponents;
                                           ret.oneOfComponents.insert(
                                               ret.oneOfComponents.end(),
                                               tmp.begin(), tmp.end());
                                        },
                                    },
                                    parent);
                    }},
                ret);
   return ret;
}

inline base::musicDevice::description::sound::Parameter
base::musicDevice::description::sound::paramInherit(
    const Parameter& parent, const Parameter& child) noexcept
{
   Parameter ret = child;
   if (parent.component && !ret.component)
      ret.component = parent.component;
   if (parent.role && !ret.role)
      ret.role = parent.role;
   if (parent.defaultValue && !ret.defaultValue)
      ret.defaultValue = parent.defaultValue;
   if (parent.source.midi)
   {
      if (parent.type == Parameter::Type::List &&
          ret.type == Parameter::Type::List)
      {
         assert(ret.source.midi->sourceRanges ||
                ret.source.midi->sourceRangesFrom);
         assert(parent.source.midi->sourceRanges ||
                parent.source.midi->sourceRangesFrom);
         if (parent.source.midi->sourceRangesFrom &&
             !ret.source.midi->sourceRangesFrom)
            ret.source.midi->sourceRangesFrom =
                parent.source.midi->sourceRangesFrom;
         const auto tmp                = *ret.source.midi->sourceRanges;
         ret.source.midi->sourceRanges = parent.source.midi->sourceRanges;
         ret.source.midi->sourceRanges->insert(
             ret.source.midi->sourceRanges->end(), tmp.begin(), tmp.end());
      }
   }

   return ret;
}

inline std::optional<int>
base::musicDevice::description::sound::Section::getParameterIdx(
    int voiceIdx, const std::string& component,
    const std::string& parameter) const noexcept
{
   const auto& parameters = engines[voice2EngineIdx(voiceIdx)].parameters;
   for (int i = 0; i < parameters.size(); ++i)
   {
      if (parameters[i].component &&
          parameters[i].component.value() == component &&
          parameters[i].name == parameter)
      {
         return i;
      }
   }
   return std::nullopt;
}

inline bool base::musicDevice::description::sound::EngineBase::canDumpPresets()
    const noexcept
{
   return presets && parameterDumpRequest;
}

inline bool base::musicDevice::description::sound::Section::canDumpPresets()
    const noexcept
{
   bool canDumpPresets = false;
   forEachEngineBase(
       [&canDumpPresets](int engineIdx, const EngineBase& rEngineBase) {
          if (rEngineBase.canDumpPresets())
          {
             canDumpPresets = true;
          }
       });
   return canDumpPresets;
}

#endif