#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_H
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "SdVoiceRef.h"
#include "ComponentData.h"

#include "CallbackSignal.h"
#include "InstrumentsParameterCache.h"
#include "MusicDevice.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"
#include "ParameterLimiter.h"
#include "function_ref.h"

namespace base::instruments::rt
{
class KitComponent
{
public:
   friend class ParameterHandler;
   explicit KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                      musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                      int noteOffset, float amp) noexcept;
   bool setNoteOffset(int noteOffset);
   bool setAmp(float amp, float prevAmp);
   void noteOn(int note, float velocity, util::function_ref<void()> refreshParameters) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   [[nodiscard]] Ret<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] Ret<float> getSDParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] Ret<float> fromNormalizedValue(
       int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   Ret<const musicDevice::description::sound::Parameter*> parameterDescription(
       int parameterIdx) const;

   Void clearModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   Void applyModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);

   [[nodiscard]] int noteOffset() const { return data.noteOffset; };
   [[nodiscard]] float amp() const { return data.amp; };

   void updateParameterUI();

   SdVoiceRef sdVoiceRef;
   ComponentData data;
};

}   // namespace base::instruments::rt

#endif
