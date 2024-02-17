#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_H
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_H

#include <magic_enum.hpp>
#include <memory>
#include <vector>

#include "CallbackSignal.h"
#include "MusicDevice.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"
#include "ErrorHandling.h"

namespace base::instruments::loader
{
class KitComponent
{
public:
   KitComponent() = default;
   explicit KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                         std::size_t numParameter,
                         musicDevice::MusicDeviceId soundDeviceId,
                         int sdVoiceIdx, int noteOffset) noexcept;
   // [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   // [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   void setNoteOffset(int noteOffset);
   void setAmp(float amp);

   [[nodiscard]] Ret<float> getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   Void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);

   Ret<const musicDevice::description::sound::Parameter*> parameterDescription(
       int parameterIdx) const;

   friend auto meta::registerMembers<KitComponent>();
   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;

   bool operator==(const KitComponent& rhs) const;

   [[nodiscard]] int noteOffset() const { return m_noteOffset; };
   [[nodiscard]] float amp() const { return m_amp; };

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   using ParameterData = musicDevice::sound::ParameterData;
   std::vector<ParameterData> m_parameterData;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_sdVoiceIdx{0};
   int m_noteOffset{0};
   float m_amp{1.0f};
};

}   // namespace base::instruments::loader

#include "KitInstrumentComponent.inl"
#include "KitInstrumentComponentMeta.h"

#endif
