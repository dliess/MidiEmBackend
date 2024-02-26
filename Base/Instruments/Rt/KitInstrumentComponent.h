#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_H
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "CallbackSignal.h"
#include "InstrumentsParameterCache.h"
#include "MusicDevice.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"

namespace base::instruments::rt
{
class KitComponent
{
public:
   explicit KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                      musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                      int noteOffset) noexcept;
   // [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   // [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   bool setNoteOffset(int noteOffset);
   bool setAmp(float amp, float prevAmp);
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   Void incrementParameterValue(int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode);
   Void incrementParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, musicDevice::sound::IncrementMode incrementMode);

   [[nodiscard]] Ret<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] Ret<float> getSDParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   Void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);
   Void setParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float value);
   [[nodiscard]] Ret<float> fromNormalizedValue(
       int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   Void clearModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   Void applyModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);

   [[nodiscard]] int noteOffset() const { return m_noteOffset; };
   [[nodiscard]] float amp() const { return m_amp; };

   void updateParameterUI();

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   ParameterCache m_parameterCache;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_sdVoiceIdx{0};
   int m_noteOffset{0};
   float m_amp{1.0f};
   Void refreshParameters() const;
};

}   // namespace base::instruments::rt

#include "KitInstrumentComponent.inl"

#endif
