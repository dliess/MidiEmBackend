#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include <memory>
#include <vector>

#include "MusicDevice.h"
#include "ParameterData.h"
#include "SoundHandler.h"
#include "CallbackSignal.h"

namespace base::instruments
{
class Voice
{
public:
   struct ParameterCache
   {
      explicit ParameterCache(size_t size) : data(size) {}
      [[nodiscard]] std::size_t size() const { return data.size(); }
      [[nodiscard]] const base::musicDevice::sound::ParameterData& at(std::size_t pos) const { return data.at(pos); }
      [[nodiscard]] base::musicDevice::sound::ParameterData& at(std::size_t pos) { return data.at(pos); }
      
      void setParameter(std::size_t index, musicDevice::sound::ParameterPart, float value);
      CB_SIGNAL_SINGLE_SUBSCRIBER(DataChanged, int, musicDevice::sound::ParameterPart, float);
      private:
      std::vector<base::musicDevice::sound::ParameterData> data;
   };
   Voice() = default;
   explicit Voice(musicDevice::sound::SoundHandler* pSoundDevice,
                  std::shared_ptr<ParameterCache> pParameterCache,
                  musicDevice::MusicDeviceId soundDeviceId, int voiceIndex,
                  int noteOffset) noexcept;
   [[nodiscard]] const musicDevice::MusicDeviceId& soundDeviceId() const;
   [[nodiscard]] const musicDevice::sound::SoundHandler* pSoundDevice() const;
   void setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr);
   void setNoteOffset(int noteOffset);
   void noteOn(int note, float velocity) const;
   void noteOff(int note, float velocity) const;
   void pitchBend(float value) const;

   void incrementParameterValue(int parameterIdx, float increment,
                                bool roundRobin) const;
   void incrementParameterValueDontCache(int parameterIdx, float increment,
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

   inline bool operator==(const Voice& rhs) const;

private:
   musicDevice::sound::SoundHandler* m_pSoundDevice{nullptr};
   std::shared_ptr<ParameterCache> m_pParameterCache;
   musicDevice::MusicDeviceId m_soundDeviceId;
   int m_voiceIndex{0};
   int m_noteOffset{0};
   void refreshParameters() const;
};

}   // namespace base::instruments

#include "InstrumentVoice.inl"
#include "InstrumentVoiceMeta.h"

#endif
