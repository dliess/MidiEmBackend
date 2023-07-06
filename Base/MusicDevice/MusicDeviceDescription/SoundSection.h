#ifndef SOUND_SECTION_H
#define SOUND_SECTION_H

#include <JsonCast.h>

#include <mpark/variant.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "EnumReflect.h"
#include "MidiMessageIds.h"
#include "Parameter.h"
#include "SoundSysExDescription.h"
namespace base::musicDevice::description::sound
{
constexpr int GlobalSectionId = -1;

using SysExDescriptors = std::vector<midisysex::FieldDescr>;
struct ParameterDumpAnswer
{
   SysExDescriptors sysexDescriptors;
};

struct MidiCCAndValue
{
   std::vector<uint8_t> cc;
   uint8_t value;
};

struct MidiSysexMsg
{
   SysExDescriptors sysexDescriptors;
};

using ParameterDumpRequestMsg = mpark::variant<MidiCCAndValue, MidiSysexMsg>;

DECLARE_ENUM(ParameterDumpRequestEffect, uint, PerVoice, AllVoicesOfEngine);

struct ParameterDumpRequest
{
   using Effect = ParameterDumpRequestEffect;
   Effect effect;
   ParameterDumpRequestMsg message;
};

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

struct MidiChannelHolder
{
   int midiChannel;
};

struct Voice : public MidiChannelHolder
{
   std::string name;
   int engineId;
   std::optional<int> midiTriggerNoteNumber;
};

DECLARE_ENUM(ComponentRole, uint, Unknown, Track, NoteTrigger, Oscillator, Amp,
             Filter, LPFilter, HPFilter, LPHPFilter, LPHPFilter2, Envelope, LFO,
             Arpeggiator, Sequencer, Sample, Effects, Equalizer, Delay, Reverb,
             Chorus, Distortion, Compressor, ModMatrix, Tempo, Mixer);

struct Component
{
   std::string name;
   using Role = ComponentRole;
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

DECLARE_ENUM(ParameterSourceRangeBaseRoles, uint, Unknown, Off, On,
             FilterLowpass2db, FilterLowpass4db, FilterHighpass2db,
             FilterHighpass4db, FilterBandpass2db, FilterBandpass4db,
             FilterPeak, FilterBandReject, FilterTwoPole, FilterFourPole,
             WaveFormSine, WaveFormAsymSine, WaveFormSawtooth,
             WaveFormSinetooth, WaveFormTriangle, WaveFormSquare,
             WaveFormPulsewidth, WaveFormRandom, WaveFormSequence, WaveTable);
struct ParameterSourceRangeBase
{
   using Role = ParameterSourceRangeBaseRoles;
   std::string name;
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

DECLARE_ENUM(
    ParameterRole, uint, Unknown, TrackVolume, TrigChance, Swing, Mute, Solo,
    KeyPrioMode, Pitch, PitchFinetune, Detune, OSCWaveform, OSCShape, OSCGlide,
    OSCGlideOnOff, OSCGlideType, OSCKeyboardTracking, OSCSync, OSCSlop, OSCMix,
    OSCNoise, OSCNoteSyncOnOff, OSC1Volume, OSC2Volume, RingModVolume,
    OSC1Decay, OSC2Decay, TransientDecay, HitDecay, SubOSCLevel, NoiseLevel,
    NoiseDecay, NoiseColor, UnisonMode, UnisonOnOff, FMAmount, InitialVolume,
    Volume, Pan, DelaySend, ReverbSend, FeedbackVolume, FeedbackGain,
    FilterCutoff, FilterBase, FilterWidth, FilterType, FilterResonance,
    FilterSlope, FilterKeyAmount, FilterAudioModuation, LPFilterCutoff,
    LPFilterResonance, LPFilterSlope, LPFilterKeyAmount, LPFilterAudioModuation,
    LPFilterSweep, ComponentSelector, HPFilterCutoff, HPFilterResonance,
    HPFilterSlope, HPFilterKeyAmount, HPFilterAudioModuation, LPHPFilterCutoff,
    LPHPFilterResonance, LPHPFilterSlope, LPHPFilterKeyAmount,
    LPHPFilterAudioModuation, AmpEnvAmount, EnvAmount, EnvVelAmount, EnvRepeat,
    EnvDelay, EnvDestination, Attack, Decay, Sustain, Release, Hold, SweepTime,
    SweepDepth, LFOAmount, LFOFrequency, LFOMultiplier, LFOWaveform,
    LFODestination, LFODepth, LFOKeySync, ArpOnOff, ArpMode, SequencerOnOff,
    SequenceTrig, SampleStart, SampleEnd, SampleLength, SampleLoop,
    SampleReverse, SampleRate, SampleBitReduction, SampleRateReduction,
    SampleSlot, ModSource, ModDestination, ModAmount, PitchBendRange, BPM,
    ClockDivide, FrequencyBandCenter, FrequencyBandAmp, FrequencyBandWidth,
    LowShelfFrequency, HighShelfFrequency, LowBandAmp, MidBandAmp, HighBandAmp,
    RetrigNum, RetrigTime, StereoWidth, DryWetMix, DelayAmnt, MakeupGain,
    Threshold, Distortion, DelayTime, DampingFactor, GateTime, ShelvingGain,
    ShelvingFrequency, PreFxLevel, PostFxLevel);

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
   using Role = ParameterRole;
   std::optional<Role> role;
   std::optional<float> defaultValue;
   std::optional<std::string> description;
   ParameterSource source;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
   static inline std::string role2String(Role role);
   static inline Role roleFromString(const std::string& roleStr);

   [[nodiscard]] int getListIndexByValue(int value) const noexcept;
   [[nodiscard]] std::optional<float> getListIndexByListRole(
       ParameterSourceRangeBase::Role role) const noexcept;
   [[nodiscard]] int getSourceResolution() const noexcept;
   [[nodiscard]] std::optional<ValueRangeEnd> getValueRange() const noexcept;
};

struct Presets
{
   int numberOfPresets;
};

struct EngineBase
{
   std::optional<std::string> from;
   std::optional<std::vector<ComponentVar>> components;
   std::vector<Parameter> parameters;
   std::optional<Presets> presets;
   std::optional<ParameterDumpRequest> parameterDumpRequest;
   std::optional<ParameterDumpAnswer> parameterDumpAnswer;
   [[nodiscard]] inline bool canDumpPresets() const noexcept;
};

struct Global : public EngineBase, public MidiChannelHolder
{
};


struct Engine : public EngineBase
{
   std::string name;
   std::optional<NoteSettings> noteSettings;
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
   std::optional<int> maxPitchBendRange;
   std::vector<Voice> voices;
   std::optional<Global> global;
   std::vector<Engine> engines;
   std::optional<std::unordered_map<std::string, Engine>> engineTemplates;
   std::optional<
       std::unordered_map<std::string, std::vector<ParameterSourceRangeMidi>>>
       sourceRanges;

   static inline std::string defaultInstrumentType2String(
       DefaultInstrumentType type);
   static inline DefaultInstrumentType defaultInstrumentTypeFromString(
       const std::string& str);

   //---------------------------------------------------
   //------------- Additional helpers ------------------
   //---------------------------------------------------

   inline const Parameter& parameterDescription(
       const ParameterId& parameterId) const noexcept;
   inline const Parameter& parameterDescription(int voiceId,
                                          int parameterId) const noexcept;
   inline const EngineBase* engineBase(int voiceId) const noexcept;
   inline const MidiChannelHolder* midiChannel(int voiceId) const noexcept;
   template<typename Cb>
   void forEachEngineBase(Cb&& cb);
   template<typename Cb>
   void forEachEngineBase(Cb&& cb) const;
   template<typename Cb>
   void forEachVoiceOfEngine(int engineIdx, Cb&& cb) const;
   inline Engine* findParentEngineByName(const std::string& name) noexcept;
   inline int getMidiChannel(int voiceId) const noexcept;
   inline std::optional<int> getVoiceIdx(int midiChannelNr) const noexcept;
   template <typename T> void forEachParameterDescr(T&& cb) noexcept;
   template <typename T> void forEachParameterDescr(T&& cb) const noexcept;
   template <typename T>
   void forEachParameterDescr(int engineIdx, T&& cb) noexcept;
   template <typename T>
   void forEachParameterDescr(int engineIdx, T&& cb) const noexcept;
   template <typename T> void forEachComponentDescr(T&& cb) noexcept;
   template <typename T> void forEachComponentDescr(T&& cb) const noexcept;
   template <typename T>
   void forEachComponentDescr(int engineIdx, T&& cb) noexcept;
   template <typename T>
   void forEachComponentDescr(int engineIdx, T&& cb) const noexcept;
   inline bool hasParameters() const noexcept;
   inline int voice2EngineIdx(int voiceIdx) const noexcept;
   template <typename T>
   static int linSearchByName(const std::vector<T>& vector,
                              const std::string& name) noexcept;
   inline float getInitialValueFor(int voiceId, int parameterId) const noexcept;

   inline void handleEngineInheritance() noexcept;
   inline void autoFillSourceRangesForLists() noexcept;
   inline void fillParameterDumpOffsetCaches() noexcept;

   inline bool isValidVoiceIdx(int VoiceIdx) const noexcept;

   inline std::optional<int> getParameterIdx(
       int voiceIdx, const std::string& component,
       const std::string& parameter) const noexcept;

   inline bool canDumpPresets() const noexcept;
private:
   inline mpark::variant<int, double, ParameterSourceRangeBase::Role>
   _getInitialValueFor(int voiceId, int parameterId) const noexcept;
   inline Engine inherit(const Engine& parent, const Engine& child) noexcept;

};

inline ComponentVar compInherit(const ComponentVar& parent,
                                const ComponentVar& child) noexcept;
inline Parameter paramInherit(const Parameter& parent,
                              const Parameter& child) noexcept;

}   // namespace base::musicDevice::description::sound

#include "SoundSection.inl"
#include "SoundSectionMeta.h"

#endif