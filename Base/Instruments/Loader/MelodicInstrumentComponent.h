#ifndef MELODIC_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_H
#define MELODIC_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "CallbackSignal.h"
#include "MusicDevice.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"

namespace base::instruments::loader
{
class MelodicComponent
{
public:
   MelodicComponent() = default;
   explicit MelodicComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                      musicDevice::MusicDeviceId soundDeviceId, int sdVoiceIdx,
                      int noteOffset) noexcept;
   // [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   // [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   void setNoteOffset(int noteOffset);
   void setAmp(float amp, float prevAmp);

   [[nodiscard]] std::optional<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   friend auto meta::registerMembers<MelodicComponent>();
   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;

   bool operator==(const MelodicComponent& rhs) const;

   [[nodiscard]] int noteOffset() const { return m_noteOffset; };
   [[nodiscard]] float amp() const { return m_amp; };

   CB_SIGNAL_SINGLE_SUBSCRIBER(NoteOffsetChanged, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(AmpChanged, float);

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   musicDevice::MusicDeviceId m_soundDeviceId;
};

}   // namespace base::instruments::loader

#include "MelodicInstrumentComponent.inl"
#include "MelodicInstrumentComponentMeta.h"

#endif
