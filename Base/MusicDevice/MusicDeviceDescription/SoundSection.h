#ifndef SOUND_SECTION_H
#define SOUND_SECTION_H

#include <JsonCast.h> // needed because of "JsonCastNamespaceFix.h"

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

// This is just a helper struct, don't reflect
struct CacheHelpers
{
   std::optional<std::string> name;
   std::optional<int> index;
};

struct NoteRange
{
   int from;
   int to;
};

struct NoteSettingsMidiRouting
{
   static constexpr int UNSET = -1;
   int destinationParameterIdx{UNSET}; // not to reflect
   std::string destinationParameter;
   std::optional<std::vector<int>> mapping;
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
};

struct Voice
{
   std::string name;
   int engineId;
   int midiChannel;
   std::optional<int> midiTriggerNoteNumber;
};

struct Component : public CacheHelpers
{
   enum class Type
   { // Todo: maybe string is enough?
      Unknown,
      Filter,
      Envelope,
      Arpeggiator
   };
   std::optional<Type> type;
   std::vector<std::string> parameterIds;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};

struct ParameterSourceRange
{
   std::string name;
   std::array<float, 2> range;
};

struct ParameterSourceMidi
{
   midi::MidiMessageId id;
   std::optional<std::vector<ParameterSourceRange>> sourceRanges;
};

struct ParameterSource
{
   std::optional<ParameterSourceMidi> midi;
};

struct Parameter : public CacheHelpers
{
   enum class Type
   {
      Continous,
      ContinousBipolar,
      List
   };
   Type type;
   std::optional<std::string> component;
   std::optional<std::string> role;
   std::optional<std::string> description;
   ParameterSource source;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};
struct Global
{
   int midiChannel;
   std::optional<std::unordered_map<std::string, Component>> components;
   std::unordered_map<std::string, Parameter> parameters;
   std::vector<std::reference_wrapper<Component>>
      componentsCached; // not to reflect
   std::vector<std::reference_wrapper<Parameter>>
      parametersCached; // not to reflect
};

struct Engine
{
   std::string name;
   std::optional<NoteSettings> noteSettings;
   std::optional<std::unordered_map<std::string, Component>> components;
   std::unordered_map<std::string, Parameter> parameters;
   std::vector<std::reference_wrapper<Component>>
      componentsCached; // not to reflect
   std::vector<std::reference_wrapper<Parameter>>
      parametersCached; // not to reflect
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

   std::optional<ParameterDumpRequest> parameterDumpRequest;
   std::optional<float> pitchBendFactor;

   static inline std::string defaultInstrumentType2String(
      DefaultInstrumentType type);
   static inline DefaultInstrumentType defaultInstrumentTypeFromString(
      const std::string& str);

   //---------------------------------------------------
   //------------- Additional helpers ------------------
   //---------------------------------------------------

   inline const Parameter& parameterDescr(int voiceId,
                                          int parameterId) const noexcept;
   inline int getMidiChannel(int voiceId) const noexcept;
   template<typename T>
   void forEachParameterDescr(T&& cb) noexcept;
   template<typename T>
   void forEachParameterDescr(T&& cb) const noexcept;
   inline bool hasParameters() const noexcept;
   inline int voice2EngineIdx(int voiceIdx) const noexcept;
};

} // namespace base::musicDevice::description::sound

#include "SoundSection.inl"
#include "SoundSectionMeta.h"

#endif