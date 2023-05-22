#ifndef INSTRUMENTS_INSTRUMENT_VOICE_H
#define INSTRUMENTS_INSTRUMENT_VOICE_H

#include <memory>
#include <vector>

#include "CallbackSignal.h"
#include "DirtyFlags.h"
#include "MusicDevice.h"
#include "ParameterData.h"
#include "SoundHandler.h"

namespace base::instruments
{
class Voice
{
public:
   struct ParameterCache
   {
      explicit ParameterCache(size_t size) : data_(size), dirtyFlags_(size) {}
      [[nodiscard]] std::size_t size() const { return data_.size(); }
      using ParameterData = base::musicDevice::sound::ParameterData;
      using DirtyFlags    = base::musicDevice::sound::DirtyFlagsVec;
      [[nodiscard]] const ParameterData& at(std::size_t pos) const
      {
         return data_.at(pos);
      }
      void setParameter(std::size_t index,
                        musicDevice::sound::ParameterAttr parameterAttr,
                        float value)
      {
         musicDevice::sound::setParameterData(data_.at(index), parameterAttr,
                                              value);
         dirtyFlags_.set(index, parameterAttr);
      }
      void updateParameterUI()
      {
         if (dirtyFlags_.any())
         {
            dirtyFlags_.forEach([this](size_t paramIdx, musicDevice::sound::ParameterAttr parameterAttr) {
               emitDataChangedUI(paramIdx, parameterAttr,
                                 musicDevice::sound::getParameterData(
                                     data_.at(paramIdx), parameterAttr));
            });
            dirtyFlags_.reset();
         }
      }
      CB_SIGNAL_SINGLE_SUBSCRIBER(DataChangedUI, int,
                                  musicDevice::sound::ParameterAttr, float);

   private:
      std::vector<ParameterData> data_;
      DirtyFlags dirtyFlags_;
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

   void incrementParameterValue(int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   void incrementParameterValueDontCache(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
       float increment, bool roundRobin) const;

   [[nodiscard]] float getParameterValue(
       int parameterIdx, musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int parameterId, musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;

   const musicDevice::description::sound::Parameter* parameterDescription(
       int parameterIdx) const;

   friend auto meta::registerMembers<Voice>();
   friend class Persister;

   inline bool operator==(const Voice& rhs) const;

   void updateParameterUI() const;

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
