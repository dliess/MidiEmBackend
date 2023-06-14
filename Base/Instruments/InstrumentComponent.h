#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "InstrumentsParameterCache.h"
#include "MusicDevice.h"
#include "SoundHandler.h"

namespace base::instruments
{
class Component
{
public:
   Component() = default;
   explicit Component(musicDevice::sound::SoundHandler* pSoundDevice,
                      std::shared_ptr<ParameterCache> pParameterCache,
                      musicDevice::MusicDeviceId soundDeviceId,
                      int sdVoiceIdx, int noteOffset) noexcept;
   void setParameterCache(std::shared_ptr<ParameterCache> p);
   [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   void setNoteOffset(int noteOffset);
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   void incrementParameterValue(int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   void incrementParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, bool roundRobin) const;

   [[nodiscard]] std::optional<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] std::optional<float> getParameterValueCached(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   void setParameterValueDontCache(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   friend auto meta::registerMembers<Component>();
   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;

   bool operator==(const Component& rhs) const;

   void updateParameterUI() const;
   ParameterCache* parameterCache();
   ParameterCache* parameterCache() const;

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   std::shared_ptr<ParameterCache> m_pParameterCache;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_sdVoiceIdx{0};
   int m_noteOffset{0};
   void refreshParameters() const;
};

}   // namespace base::instruments

#include "InstrumentComponent.inl"
#include "InstrumentComponentMeta.h"

#endif
