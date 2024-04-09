#ifndef KIT_INSTRUMENT_RT_H
#define KIT_INSTRUMENT_RT_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "Identifiable.h"
#include "Instrument.h"
#include "MusicDeviceId.h"
#include "function_ref.h"
#include "ErrorHandling.h"
#include "SdVoiceRef.h"
#include "ComponentData.h"
#include "ParameterData.h"

namespace base::instruments { class KitInstrumentCopyer; }

namespace base::instruments::rt
{

class KitInstrument : public Instrument
{
public:
   static constexpr std::size_t NUM_VOICES = 16;
   explicit KitInstrument(util::Identifiable::UUIDView uuid) noexcept;
   Void noteOn(int note, float velocity, void* token = nullptr);
   Void noteOff(int note, float velocity, void* token = nullptr);
   Void noteOn(int voiceIdx, int note, float velocity,
               void* token = nullptr);
   Void noteOff(int voiceIdx, int note, float velocity,
                void* token = nullptr);

   Void incrementParameterValue(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode);
   Void incrementParameterValueEventBound(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode);

   [[nodiscard]] Ret<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   Void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);
   Void setRelativeParameterValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue);

   [[nodiscard]] Ret<float> fromNormalizedValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   Void clearModifier(int voiceIdx, int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   Void applyModifier(int voiceIdx, int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);

   [[nodiscard]] Ret<const musicDevice::description::sound::Parameter*>
   parameterDescription(int voiceIdx, int componentIdx, int parameterIdx) const;
      
   void updateParameterUI();

   Void setVoiceNoteOffset(int voiceIdx, int offset);
   Void setComponentNoteOffset(int voiceIdx, int componentIdx, int offset);

   Void setVoiceAmp(int voiceIdx, float amp);
   Void setComponentAmp(int voiceIdx, int componentIdx, float amp);


private:
   friend class base::instruments::KitInstrumentCopyer;
   friend class KitInstrumentsModifier;
   friend class KitInstrumentModifier;
   struct Voice
   {
      struct Component
      {
         using ParametersDescr = std::vector<musicDevice::description::sound::Parameter>;
         using ParameterData = std::vector<musicDevice::sound::ParameterData>;
         explicit Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                            const ParametersDescr& parametersDescr) noexcept;
         explicit Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                            const ParametersDescr& parametersDescr,
                            const ParameterData& paramData) noexcept;
         SdVoiceRef sdVoiceRef;
         ComponentData data;
      };
      static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
      std::vector<Component> components;
      int noteOffset{0};
      float amp{1.0f};
   };
   std::vector<Voice> m_voices;
   Ret<int> toVoiceIndex(int note) const;
   Ret<Voice::Component*> getComponent(int voiceIdx, int componentIdx) noexcept;
   Ret<const Voice::Component*> getComponent(int voiceIdx, int componentIdx) const noexcept;
};

} // namespace base::instruments::rt

#endif   // KIT_INSTRUMENT_RT_H
