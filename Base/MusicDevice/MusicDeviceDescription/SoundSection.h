#ifndef SOUND_SECTION_H
#define SOUND_SECTION_H

#include <JsonCast.h>   // needed because of "JsonCastNamespaceFix.h"

#include <mpark/variant.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "MidiMessageIds.h"
namespace base::musicDevice::description::sound
{
constexpr int GlobalSectionId = -1;

struct ParameterId
{
   static constexpr int UNSET = -2;
   int engineId{UNSET};
   int parameterId{UNSET};
};

struct NoteRange
{
   int from;
   int to;
};

struct NoteSettingsMidiRouting
{
   static constexpr int UNSET = -1;
   int destinationParameterIdx{UNSET};   // not to reflect
   std::string destinationParameter;
   std::optional<std::vector<float>> mapping;
};

struct NoteSettingsMidi
{
   std::optional<NoteSettingsMidiRouting> pitchRouting;
   std::optional<NoteSettingsMidiRouting> velocityRouting;
};

struct NoteSettings
{
   NoteRange noteRange;
   std::optional<NoteSettingsMidi> midi;
   std::optional<int> defaultTriggerNote;
};

struct Voice
{
   std::string name;
   int engineId;
   int midiChannel;
   std::optional<int> midiTriggerNoteNumber;
};

struct Component
{
   std::string name;
   enum class Role
   {
      Unknown,
      NoteTrigger,
      Oscillator,
      Amp,
      Filter,
      LPFilter,
      HPFilter,
      LPHPFilter,
      Envelope,
      LFO,
      Arpeggiator,
      Sequencer,
      Sample,
      Effects,
      Delay,
      Reverb,
      Chorus,
      Distortion,
      Compressor,
      ModMatrix,
      Tempo
   };
   std::optional<Role> role;
   std::optional<std::string> constParams;
   static inline std::string role2String(Role role);
   static inline Role roleFromString(const std::string& roleStr);
};

struct SelectorParameter
{
   std::string component;
   std::string name;
};

struct OneOfComponents
{
   std::string name;
   SelectorParameter selectorParam;
   std::vector<Component> oneOfComponents;
   std::optional<Component::Role> role;
};

using ComponentVar = mpark::variant<Component, OneOfComponents>;

template <typename T> struct ValueRange
{
   T from;
   T to;
};

using ValueRangeFloat = ValueRange<float>;
using ValueRangeInt   = ValueRange<int>;

struct ParameterSourceRangeBase
{
   std::string name;
   enum class Role
   {
      Unknown,
      Off,
      On,
      FilterLowpass2db,
      FilterLowpass4db,
      FilterHighpass2db,
      FilterHighpass4db,
      FilterBandpass2db,
      FilterBandpass4db,
      FilterPeak,
      FilterBandReject,
      FilterTwoPole,
      FilterFourPole,
      WaveFormSawtooth,
      WaveFormTriangle,
      WaveFormSquare,
      WaveFormPulsewidth
   };
   std::optional<Role> role;
   static inline std::string role2String(Role role);
   static inline Role roleFromString(const std::string& roleStr);
};

struct ParameterSourceRangeMidi : public ParameterSourceRangeBase
{
   ValueRangeInt range;
};

struct ParameterSourceMidi
{
   midi::MidiMessageId id;
   std::optional<ValueRangeInt> sourceValueRange;
   std::optional<std::string> sourceRangesFrom;
   std::optional<std::vector<ParameterSourceRangeMidi>> sourceRanges;
};

struct ParameterSource
{
   std::optional<ParameterSourceMidi> midi;
};

struct Parameter
{
   std::string name;
   enum class Type
   {
      Continous,
      ContinousBipolar,
      List
   };
   Type type;
   std::optional<std::string> component;
   enum class Role
   {
      Unknown,
      TrackVolume,
      TrigChance,
      Swing,
      Mute,
      Solo,
      KeyPrioMode,
      Pitch,
      PitchFinetune,
      OSCWaveform,
      OSCShape,   // NEW
      OSCGlide,
      OSCGlideOnOff,   // NEW
      OSCGlideType,    // NEW
      OSCKeyboardTracking,
      OSCSync,
      OSCSlop,
      OSCMix,
      OSCNoise,
      OSCNoteSyncOnOff,   // NEW
      SubOSCLevel,
      UnisonMode,
      UnisonOnOff,
      InitialVolume,
      Volume,
      Pan,
      DelaySend,
      ReverbSend,
      FeedbackVolume,
      FeedbackGain,
      FilterCutoff,
      FilterType,
      FilterResonance,
      FilterSlope,
      FilterKeyAmount,
      FilterAudioModuation,
      LPFilterCutoff,
      LPFilterResonance,
      LPFilterSlope,
      LPFilterKeyAmount,
      LPFilterAudioModuation,
      LPFilterSweep,
      ComponentSelector,
      HPFilterCutoff,
      HPFilterResonance,
      HPFilterSlope,
      HPFilterKeyAmount,
      HPFilterAudioModuation,
      LPHPFilterCutoff,
      LPHPFilterResonance,
      LPHPFilterSlope,
      LPHPFilterKeyAmount,
      LPHPFilterAudioModuation,
      AmpEnvAmount,
      EnvAmount,
      EnvVelAmount,
      EnvRepeat,
      EnvDelay,
      EnvDestination,
      Attack,
      Decay,
      Sustain,
      Release,
      LFOAmount,
      LFOSpeed,
      LFOMultiplier,
      LFOWaveform,
      LFODestination,
      LFODepth,
      LFOKeySync,
      ArpOnOff,
      ArpMode,
      SequencerOnOff,
      SequenceTrig,
      SampleStart,
      SampleEnd,
      SampleLength,
      SampleLoop,
      SampleReverse,
      SampleRate,
      SampleBitReduction,
      SampleSlot,
      ModSource,
      ModDestination,
      ModAmount,
      PitchBendRange,
      BPM,
      ClockDivide
   };
   std::optional<Role> role;
   std::optional<float> defaultValue;
   std::optional<std::string> description;
   ParameterSource source;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
   static inline std::string role2String(Role role);
   static inline Role roleFromString(const std::string& roleStr);

   inline int getListIndexByValue(int value) const noexcept;
   inline std::optional<float> getListIndexByListRole(
       ParameterSourceRangeBase::Role role) const noexcept;
   inline int getSourceResolution() const noexcept;
};

struct EngineBase
{
   std::optional<std::vector<ComponentVar>> components;
   std::vector<Parameter> parameters;
};

struct Global : public EngineBase
{
   int midiChannel;
};

struct Engine : public EngineBase
{
   std::string name;
   std::optional<NoteSettings> noteSettings;
};

struct MidiCCAndValue
{
   std::vector<uint8_t> cc;
   uint8_t value;
};

struct ParameterDumpRequest
{
   std::optional<std::vector<uint8_t>> midiSysex;
   std::optional<MidiCCAndValue> midiMsg;
};

struct Section
{
   enum class DefaultInstrumentType
   {
      DrumKit,
      InstrumentPerVoice,
      OnePolyphonicInstrument
   };
   DefaultInstrumentType defaultInstrumentType;
   std::vector<Voice> voices;
   std::optional<Global> global;
   std::vector<Engine> engines;
   std::optional<
       std::unordered_map<std::string, std::vector<ParameterSourceRangeMidi>>>
       sourceRanges;
   std::optional<ParameterDumpRequest> parameterDumpRequest;
   std::optional<float> pitchBendFactor;

   static inline std::string defaultInstrumentType2String(
       DefaultInstrumentType type);
   static inline DefaultInstrumentType defaultInstrumentTypeFromString(
       const std::string& str);

   //---------------------------------------------------
   //------------- Additional helpers ------------------
   //---------------------------------------------------

   inline const Parameter& parameterDescr(
       const ParameterId& parameterId) const noexcept;
   inline const Parameter& parameterDescr(int voiceId,
                                          int parameterId) const noexcept;
   inline const EngineBase* engineBase(int voiceId) const noexcept;
   inline int getMidiChannel(int voiceId) const noexcept;
   template <typename T> void forEachParameterDescr(T&& cb) noexcept;
   template <typename T> void forEachParameterDescr(T&& cb) const noexcept;
   template <typename T> void forEachParameterDescr(int engineIdx, T&& cb) noexcept;
   template <typename T> void forEachParameterDescr(int engineIdx, T&& cb) const noexcept;
   template <typename T> void forEachComponentDescr(T&& cb) noexcept;
   template <typename T> void forEachComponentDescr(T&& cb) const noexcept;
   template <typename T> void forEachComponentDescr(int engineIdx, T&& cb) noexcept;
   template <typename T> void forEachComponentDescr(int engineIdx, T&& cb) const noexcept;
   inline bool hasParameters() const noexcept;
   inline int voice2EngineIdx(int voiceIdx) const noexcept;
   template <typename T>
   static int linSearchByName(const std::vector<T>& vector,
                              const std::string& name) noexcept;
   inline float getInitialValueFor(int voiceId, int parameterId) const noexcept;

private:
   inline mpark::variant<float, ParameterSourceRangeBase::Role>
   _getInitialValueFor(int voiceId, int parameterId) const noexcept;
};

}   // namespace base::musicDevice::description::sound

#include "SoundSection.inl"
#include "SoundSectionMeta.h"

#endif