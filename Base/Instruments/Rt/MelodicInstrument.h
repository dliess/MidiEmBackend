#ifndef MELODIC_INSTRUMENT_RT_H
#define MELODIC_INSTRUMENT_RT_H

#include <array>
#include <memory>
#include <vector>

#include "Identifiable.h"
#include "Instrument.h"
#include "InstrumentsParameterCache.h"
#include "MelodicInstrumentVoice.h"
#include "MusicDeviceId.h"
#include "NoteAllocation.h"
#include "ComponentData.h"
#include "SdVoiceRef.h"

namespace base::instruments { class MelodicInstrumentCopyer; }

namespace base::instruments::rt
{
class MelodicInstrument : public Instrument
{
public:

   MelodicInstrument(util::Identifiable::UUIDView uuid) noexcept;

   Void noteOn(int note, float velocity, void* token = nullptr);
   Void noteOff(int note, float velocity, void* token = nullptr);

   void pitchBend(float value) const;
   Void pitchBendMPE(int note, float value);
   Void incrementParameterValue(int componentIdx, int parameterId,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode);
   Void incrementParameterValueEventBound(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float increment,
       musicDevice::sound::IncrementMode incrementMode);

   Void incrementParameterValueMPE(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float increment,
       musicDevice::sound::IncrementMode incrementMode);
   [[nodiscard]] Ret<float> getParameterValue(
       int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] Ret<float> getParameterValue(
       int note, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   Void setParameterValue(int componentIdx, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);
   Void setRelativeParameterValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue);

   Void setParameterValueMPE(int note, int componentIdx, int parameterId,
                             musicDevice::sound::ParameterAttr parameterAttr,
                             float value);
   [[nodiscard]] Ret<float> fromNormalizedValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   [[nodiscard]] Ret<float> fromNormalizedValue(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   // unused yet
   Void clearModifier(int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   // unused yet
   Void applyModifier(int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);

   [[nodiscard]] Ret<const musicDevice::description::sound::Parameter*>
      parameterDescription(int componentIdx, int parameterIdx) const;

   void updateParameterUI();

   // [[nodiscard]] const Component* getFirstComponent(size_t componentIdx) const;
   
private:
   friend class base::instruments::MelodicInstrumentCopyer;
   friend class MelodicInstrumentModifier;
   static constexpr std::size_t NUM_COMPONENTS = 4;
   using Voice = std::array<std::optional<SdVoiceRef>, NUM_COMPONENTS>;
   std::vector<Voice> m_voices;
   std::array<std::optional<ComponentData>, NUM_COMPONENTS> m_engines;
   mutable NoteAllocation m_noteAllocation;
};

}   // namespace base::instruments::rt

// #include "MelodicInstrument.inl"

#endif   // MELODIC_INSTRUMENT_RT_H
