#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "CallbackSignal.h"
#include "InstrumentsParameterCache.h"
#include "MusicDevice.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"

namespace base::instruments
{
class Component
{
public:
   Component() = default;
   explicit Component(musicDevice::sound::SoundHandler* pSoundDevice,
                      std::shared_ptr<ParameterCache> pParameterCache,
                      musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                      int noteOffset) noexcept;
   void setParameterCache(std::shared_ptr<ParameterCache> p);
   [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   void setNoteOffset(int noteOffset);
   void setAmp(float amp, float prevAmp);
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   void incrementParameterValue(int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) const;
   void incrementParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, musicDevice::sound::IncrementMode incrementMode) const;

   [[nodiscard]] std::optional<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] std::optional<float> getSDParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   void setParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float value) const;
   [[nodiscard]] float fromNormalizedValue(
       int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   void clearModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr) const;
   void applyModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity) const;

   friend auto meta::registerMembers<Component>();
   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;

   bool operator==(const Component& rhs) const;

   [[nodiscard]] int noteOffset() const { return m_noteOffset; };
   [[nodiscard]] float amp() const { return m_amp; };

   void updateParameterUI() const;
   ParameterCache* parameterCache();
   ParameterCache* parameterCache() const;

   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOffsetChanged, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(AmpChanged, float);

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   std::shared_ptr<ParameterCache> m_pParameterCache;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_sdVoiceIdx{0};
   int m_noteOffset{0};
   float m_amp{1.0f};
   void refreshParameters() const;
};

}   // namespace base::instruments

#include "InstrumentComponent.inl"
#include "InstrumentComponentMeta.h"

#endif
