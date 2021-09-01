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
      case Role::Oscillator: return "Oscillator";
      case Role::Amp: return "Amp";
      case Role::Filter: return "Filter";
      case Role::LPFilter: return "LPFilter";
      case Role::HPFilter: return "HPFilter";
      case Role::LPHPFilter: return "LPHPFilter";
      case Role::Envelope: return "Envelope";
      case Role::LFO: return "LFO";
      case Role::Arpeggiator: return "Arpeggiator";
      case Role::Sequencer: return "Sequencer";
      case Role::Sample: return "Sample";
      case Role::Effects: return "Effects";
      case Role::Delay: return "Delay";
      case Role::Reverb: return "Reverb";
      case Role::Chorus: return "Chorus";
      case Role::Distortion: return "Distortion";
      case Role::Compressor: return "Compressor";
      case Role::ModMatrix: return "ModMatrix";
      case Role::Tempo: return "Tempo";
   }
   return "Unknown";
}

inline base::musicDevice::description::sound::Component::Role
base::musicDevice::description::sound::Component::roleFromString(
    const std::string& roleStr)
{
   if (roleStr == "NoteTrigger")
      return Role::NoteTrigger;
   else if (roleStr == "Oscillator")
      return Role::Oscillator;
   else if (roleStr == "Amp")
      return Role::Amp;
   else if (roleStr == "Filter")
      return Role::Filter;
   else if (roleStr == "LPFilter")
      return Role::LPFilter;
   else if (roleStr == "HPFilter")
      return Role::HPFilter;
   else if (roleStr == "LPHPFilter")
      return Role::LPHPFilter;
   else if (roleStr == "Envelope")
      return Role::Envelope;
   else if (roleStr == "LFO")
      return Role::LFO;
   else if (roleStr == "Arpeggiator")
      return Role::Arpeggiator;
   else if (roleStr == "Sequencer")
      return Role::Sequencer;
   else if (roleStr == "Sample")
      return Role::Sample;
   else if (roleStr == "Effects")
      return Role::Effects;
   else if (roleStr == "Delay")
      return Role::Delay;
   else if (roleStr == "Reverb")
      return Role::Reverb;
   else if (roleStr == "Chorus")
      return Role::Chorus;
   else if (roleStr == "Distortion")
      return Role::Distortion;
   else if (roleStr == "Compressor")
      return Role::Compressor;
   else if (roleStr == "ModMatrix")
      return Role::ModMatrix;
   else if (roleStr == "Tempo")
      return Role::Tempo;
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
   switch(role)
   {
      case Role::Unknown: return "Unknown";
      case Role::TrackVolume: return "TrackVolume";
      case Role::TrigChance: return "TrigChance";
      case Role::Swing: return "Swing";
      case Role::Mute: return "Mute";
      case Role::Solo: return "Solo";
      case Role::KeyPrioMode: return "KeyPrioMode";
      case Role::Pitch: return "Pitch";
      case Role::PitchFinetune: return "PitchFinetune";
      case Role::OSCWaveform: return "OSCWaveform";
      case Role::OSCGlide: return "OSCGlide";
      case Role::OSCKeyboardTracking: return "OSCKeyboardTracking";
      case Role::OSCSync: return "OSCSync";
      case Role::OSCSlop: return "OSCSlop";
      case Role::OSCMix: return "OSCMix";
      case Role::OSCNoise: return "OSCNoise";
      case Role::SubOSCLevel: return "SubOSCLevel";
      case Role::UnisonMode: return "UnisonMode";
      case Role::UnisonOnOff: return "UnisonOnOff";
      case Role::InitialVolume: return "InitialVolume";
      case Role::Volume: return "Volume";
      case Role::Pan: return "Pan";
      case Role::DelaySend: return "DelaySend";
      case Role::ReverbSend: return "ReverbSend";
      case Role::FeedbackVolume: return "FeedbackVolume";
      case Role::FeedbackGain: return "FeedbackGain";
      case Role::FilterCutoff: return "FilterCutoff";
      case Role::FilterType: return "FilterType";
      case Role::FilterResonance: return "FilterResonance";
      case Role::FilterSlope: return "FilterSlope";
      case Role::FilterKeyAmount: return "FilterKeyAmount";
      case Role::FilterAudioModuation: return "FilterAudioModuation";
      case Role::LPFilterCutoff: return "LPFilterCutoff";
      case Role::LPFilterResonance: return "LPFilterResonance";
      case Role::LPFilterSlope: return "LPFilterSlope";
      case Role::LPFilterKeyAmount: return "LPFilterKeyAmount";
      case Role::LPFilterAudioModuation: return "LPFilterAudioModuation";
      case Role::HPFilterCutoff: return "HPFilterCutoff";
      case Role::HPFilterResonance: return "HPFilterResonance";
      case Role::HPFilterSlope: return "HPFilterSlope";
      case Role::HPFilterKeyAmount: return "HPFilterKeyAmount";
      case Role::HPFilterAudioModuation: return "HPFilterAudioModuation";
      case Role::LPHPFilterCutoff: return "LPHPFilterCutoff";
      case Role::LPHPFilterResonance: return "LPHPFilterResonance";
      case Role::LPHPFilterSlope: return "LPHPFilterSlope";
      case Role::LPHPFilterKeyAmount: return "LPHPFilterKeyAmount";
      case Role::LPHPFilterAudioModuation: return "LPHPFilterAudioModuation";
      case Role::AmpEnvAmount: return "AmpEnvAmount";
      case Role::EnvAmount: return "EnvAmount";
      case Role::EnvVelAmount: return "EnvVelAmount";
      case Role::EnvRepeat: return "EnvRepeat";
      case Role::EnvDelay: return "EnvDelay";
      case Role::EnvDestination: return "EnvDestination";
      case Role::Attack: return "Attack";
      case Role::Decay: return "Decay";
      case Role::Sustain: return "Sustain";
      case Role::Release: return "Release";
      case Role::LFOAmount: return "LFOAmount";
      case Role::LFOSpeed: return "LFOSpeed";
      case Role::LFOMultiplier: return "LFOMultiplier";
      case Role::LFOWaveform: return "LFOWaveform";
      case Role::LFODestination: return "LFODestination";
      case Role::LFODepth: return "LFODepth";
      case Role::LFOKeySync: return "LFOKeySync";
      case Role::ArpOnOff: return "ArpOnOff";
      case Role::ArpMode: return "ArpMode";
      case Role::SequencerOnOff: return "SequencerOnOff";
      case Role::SequenceTrig: return "SequenceTrig";
      case Role::SampleStart: return "SampleStart";
      case Role::SampleEnd: return "SampleEnd";
      case Role::SampleLength: return "SampleLength";
      case Role::SampleLoop: return "SampleLoop";
      case Role::SampleReverse: return "SampleReverse";
      case Role::SampleRate: return "SampleRate";
      case Role::SampleBitReduction: return "SampleBitReduction";
      case Role::SampleSlot: return "SampleSlot";
      case Role::ModSource: return "ModSource";
      case Role::ModDestination: return "ModDestination";
      case Role::ModAmount: return "ModAmount";
      case Role::PitchBendRange: return "PitchBendRange";
      case Role::BPM: return "BPM";
      case Role::ClockDivide: return "ClockDivide";
      default: return "Unknown";
   }
}

inline base::musicDevice::description::sound::Parameter::Role
base::musicDevice::description::sound::Parameter::roleFromString(
    const std::string& roleStr)
{
   if (roleStr == "TrackVolume") return Role::TrackVolume;
   else if(roleStr == "TrigChance") return Role::TrigChance;
   else if(roleStr == "Swing") return Role::Swing;
   else if(roleStr == "Mute") return Role::Mute;
   else if(roleStr == "Solo") return Role::Solo;
   else if(roleStr == "KeyPrioMode") return Role::KeyPrioMode;
   else if(roleStr == "Pitch") return Role::Pitch;
   else if(roleStr == "PitchFinetune") return Role::PitchFinetune;
   else if(roleStr == "OSCWaveform") return Role::OSCWaveform;
   else if(roleStr == "OSCGlide") return Role::OSCGlide;
   else if(roleStr == "OSCKeyboardTracking") return Role::OSCKeyboardTracking;
   else if(roleStr == "OSCSync") return Role::OSCSync;
   else if(roleStr == "OSCSlop") return Role::OSCSlop;
   else if(roleStr == "OSCMix") return Role::OSCMix;
   else if(roleStr == "OSCNoise") return Role::OSCNoise;
   else if(roleStr == "SubOSCLevel") return Role::SubOSCLevel;
   else if(roleStr == "UnisonMode") return Role::UnisonMode;
   else if(roleStr == "UnisonOnOff") return Role::UnisonOnOff;
   else if(roleStr == "InitialVolume") return Role::InitialVolume;
   else if(roleStr == "Volume") return Role::Volume;
   else if(roleStr == "Pan") return Role::Pan;
   else if(roleStr == "DelaySend") return Role::DelaySend;
   else if(roleStr == "ReverbSend") return Role::ReverbSend;
   else if(roleStr == "FeedbackVolume") return Role::FeedbackVolume;
   else if(roleStr == "FeedbackGain") return Role::FeedbackGain;
   else if(roleStr == "FilterCutoff") return Role::FilterCutoff;
   else if(roleStr == "FilterType") return Role::FilterType;
   else if(roleStr == "FilterResonance") return Role::FilterResonance;
   else if(roleStr == "FilterSlope") return Role::FilterSlope;
   else if(roleStr == "FilterKeyAmount") return Role::FilterKeyAmount;
   else if(roleStr == "FilterAudioModuation") return Role::FilterAudioModuation;
   else if(roleStr == "LPFilterCutoff") return Role::LPFilterCutoff;
   else if(roleStr == "LPFilterResonance") return Role::LPFilterResonance;
   else if(roleStr == "LPFilterSlope") return Role::LPFilterSlope;
   else if(roleStr == "LPFilterKeyAmount") return Role::LPFilterKeyAmount;
   else if(roleStr == "LPFilterAudioModuation") return Role::LPFilterAudioModuation;
   else if(roleStr == "HPFilterCutoff") return Role::HPFilterCutoff;
   else if(roleStr == "HPFilterResonance") return Role::HPFilterResonance;
   else if(roleStr == "HPFilterSlope") return Role::HPFilterSlope;
   else if(roleStr == "HPFilterKeyAmount") return Role::HPFilterKeyAmount;
   else if(roleStr == "HPFilterAudioModuation") return Role::HPFilterAudioModuation;
   else if(roleStr == "LPHPFilterCutoff") return Role::LPHPFilterCutoff;
   else if(roleStr == "LPHPFilterResonance") return Role::LPHPFilterResonance;
   else if(roleStr == "LPHPFilterSlope") return Role::LPHPFilterSlope;
   else if(roleStr == "LPHPFilterKeyAmount") return Role::LPHPFilterKeyAmount;
   else if(roleStr == "LPHPFilterAudioModuation") return Role::LPHPFilterAudioModuation;
   else if(roleStr == "AmpEnvAmount") return Role::AmpEnvAmount;
   else if(roleStr == "EnvAmount") return Role::EnvAmount;
   else if(roleStr == "EnvVelAmount") return Role::EnvVelAmount;
   else if(roleStr == "EnvRepeat") return Role::EnvRepeat;
   else if(roleStr == "EnvDelay") return Role::EnvDelay;
   else if(roleStr == "EnvDestination") return Role::EnvDestination;
   else if(roleStr == "Attack") return Role::Attack;
   else if(roleStr == "Decay") return Role::Decay;
   else if(roleStr == "Sustain") return Role::Sustain;
   else if(roleStr == "Release") return Role::Release;
   else if(roleStr == "LFOAmount") return Role::LFOAmount;
   else if(roleStr == "LFOSpeed") return Role::LFOSpeed;
   else if(roleStr == "LFOMultiplier") return Role::LFOMultiplier;
   else if(roleStr == "LFOWaveform") return Role::LFOWaveform;
   else if(roleStr == "LFODestination") return Role::LFODestination;
   else if(roleStr == "LFODepth") return Role::LFODepth;
   else if(roleStr == "LFOKeySync") return Role::LFOKeySync;
   else if(roleStr == "ArpOnOff") return Role::ArpOnOff;
   else if(roleStr == "ArpMode") return Role::ArpMode;
   else if(roleStr == "SequencerOnOff") return Role::SequencerOnOff;
   else if(roleStr == "SequenceTrig") return Role::SequenceTrig;
   else if(roleStr == "SampleStart") return Role::SampleStart;
   else if(roleStr == "SampleEnd") return Role::SampleEnd;
   else if(roleStr == "SampleLength") return Role::SampleLength;
   else if(roleStr == "SampleLoop") return Role::SampleLoop;
   else if(roleStr == "SampleReverse") return Role::SampleReverse;
   else if(roleStr == "SampleRate") return Role::SampleRate;
   else if(roleStr == "SampleBitReduction") return Role::SampleBitReduction;
   else if(roleStr == "SampleSlot") return Role::SampleSlot;
   else if(roleStr == "ModSource") return Role::ModSource;
   else if(roleStr == "ModDestination") return Role::ModDestination;
   else if(roleStr == "ModAmount") return Role::ModAmount;
   else if(roleStr == "PitchBendRange") return Role::PitchBendRange;
   else if(roleStr == "BPM") return Role::BPM;
   else if(roleStr == "ClockDivide") return Role::ClockDivide;
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
      case Parameter::Role::TrackVolume: return 0.8;
      case Parameter::Role::TrigChance: return 0.99;
      case Parameter::Role::Swing: return 0.0;
      case Parameter::Role::Mute: return 0.0;
      case Parameter::Role::Solo: return 0.0;
      case Parameter::Role::KeyPrioMode: return 0.0;
      case Parameter::Role::Pitch: return 0.5;
      case Parameter::Role::PitchFinetune: return 0.5;
      case Parameter::Role::OSCWaveform: return 1;
      case Parameter::Role::OSCGlide: return 0.0;
      case Parameter::Role::OSCKeyboardTracking: return 2;
      case Parameter::Role::OSCSync: return 0.0;
      case Parameter::Role::OSCSlop: return 0.0;
      case Parameter::Role::OSCMix: return 0.0;
      case Parameter::Role::OSCNoise: return 0.0;
      case Parameter::Role::SubOSCLevel: return 0.0;
      case Parameter::Role::UnisonMode: return 0.0;
      case Parameter::Role::UnisonOnOff: return 0.0;
      case Parameter::Role::InitialVolume: return 0.0;
      case Parameter::Role::Volume: return 0.4;
      case Parameter::Role::Pan: return 0.5;
      case Parameter::Role::DelaySend: return 0.0;
      case Parameter::Role::ReverbSend: return 0.0;
      case Parameter::Role::FeedbackVolume: return 0.0;
      case Parameter::Role::FeedbackGain: return 0.0;
      case Parameter::Role::FilterCutoff: return 0.99;
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
      case Parameter::Role::AmpEnvAmount: return 0.99;
      case Parameter::Role::EnvAmount: return 0.5;
      case Parameter::Role::EnvVelAmount: return 0.0;
      case Parameter::Role::EnvRepeat: return 0.0;
      case Parameter::Role::EnvDelay: return 0.0;
      case Parameter::Role::EnvDestination: return 1;
      case Parameter::Role::Attack: return 0.0;
      case Parameter::Role::Decay: return 0.0;
      case Parameter::Role::Sustain: return 0.5;
      case Parameter::Role::Release: return 0.0;
      case Parameter::Role::LFOAmount: return 0.0;
      case Parameter::Role::LFOSpeed: return 0.0;
      case Parameter::Role::LFOMultiplier: return 0.0;
      case Parameter::Role::LFOWaveform: return 1;
      case Parameter::Role::LFODestination: return 1;
      case Parameter::Role::LFODepth: return 0.0;
      case Parameter::Role::LFOKeySync: return 0.0;
      case Parameter::Role::ArpOnOff: return 1;
      case Parameter::Role::ArpMode: return 0.0;
      case Parameter::Role::SequencerOnOff: return 1;
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