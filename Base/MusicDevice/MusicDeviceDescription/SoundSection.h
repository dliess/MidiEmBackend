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
   enum class Type
   {   // Todo: maybe string is enough?
      Unknown,
      Filter,
      Envelope,
      Arpeggiator
   };
   std::optional<Type> type;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};

struct ValueRange
{
   float from;
   float to;
};

struct ParameterSourceRange
{
   std::string name;
   std::optional<ValueRange> range;
};

struct ParameterSourceValueRange
{
   uint32_t from;
   uint32_t to;
};
struct ParameterSourceMidi
{
   midi::MidiMessageId id;
   std::optional<ParameterSourceValueRange> sourceValueRange;
};

struct ParameterSource
{
   std::optional<ParameterSourceMidi> midi;
   std::optional<std::vector<ParameterSourceRange>> sourceRanges;
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
   std::optional<std::string> role;
   std::optional<std::string> description;
   ParameterSource source;

   static inline std::string type2String(Type type);
   static inline Type typeFromString(const std::string& str);
};

struct EngineBase
{
   std::optional<std::vector<Component>> components;
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
   inline bool hasParameters() const noexcept;
   inline int voice2EngineIdx(int voiceIdx) const noexcept;
   template <typename T>
   static int linSearchByName(const std::vector<T>& vector,
                              const std::string& name) noexcept;
};

}   // namespace base::musicDevice::description::sound

#include "SoundSection.inl"
#include "SoundSectionMeta.h"

#endif