#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include <memory>
#include <vector>

#include "MusicDevice.h"
#include "ParameterData.h"
#include "SoundHandler.h"

namespace base::instruments
{
class Voice
{
public:
   using ParameterCache = std::vector<base::musicDevice::sound::ParameterData>;
   Voice()              = default;
   inline explicit Voice(musicDevice::sound::SoundHandler* pSoundDevice,
                         std::shared_ptr<ParameterCache> pParameterCache,
                         musicDevice::MusicDeviceId soundDeviceId,
                         int voiceIndex, int noteOffset) noexcept;
   [[nodiscard]] inline const musicDevice::MusicDeviceId& soundDeviceId() const;
   [[nodiscard]] inline const musicDevice::sound::SoundHandler* pSoundDevice()
       const;
   inline void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   inline void setNoteOffset(int noteOffset);
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   void incrementParameterValue(int parameterIdx, float increment,
                                bool roundRobin) const;
   [[nodiscard]] float getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterPart parameterPart) const;
   void setParameterValue(int parameterIdx, float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int parameterId, musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const;

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   friend auto meta::registerMembers<Voice>();
   friend class Persister;

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   std::shared_ptr<ParameterCache> m_pParameterCache;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_voiceIndex{0};
   int m_noteOffset{0};

private:
   void refreshParameters() const;
};

}   // namespace base::instruments

#include "InstrumentVoice.inl"

#endif
