#ifndef MELODIC_INSTRUMENT_H
#define MELODIC_INSTRUMENT_H

#include <array>
#include <memory>
#include <vector>

#include "CompositeSound.h"
#include "Instrument.h"
#include "MusicDeviceId.h"

class MusicDevice;

namespace base
{
namespace musicDevice
{
struct Holder;
struct MusicDevice;
}   // namespace musicDevice
namespace instruments
{
class MelodicInstrument : public Instrument
{
public:
   struct RtData
   {
      static constexpr int NUM_NOTES = 128;
      static constexpr int FREE      = -1;
      std::array<int, NUM_NOTES> noteAllocations;
      inline void incrementVoiceIndex(int maxSize) noexcept;
      [[nodiscard]] inline int currentVoiceIndex() const noexcept;

   private:
      int m_currentVoiceIndex{-1};
   };

   MelodicInstrument() : MelodicInstrument("", std::make_shared<RtData>()){};
   MelodicInstrument(std::string name, std::shared_ptr<RtData> rtData) noexcept;

   void noteOn(int note, float velocity, void* token = nullptr) const override;
   void noteOff(int note, float velocity, void* token = nullptr) const override;

   void pitchBend(float value) const;
   void incrementParameterValue(int compPart, int parameterId,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   void incrementParameterValue(int note, int compPart, int parameterId,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   [[nodiscard]] float getParameterValue(
       int compPart, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] float getParameterValue(
       int note, int compPart, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int compPart, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   void setParameterValue(int note, int compPart, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int compPart, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   [[nodiscard]] float normalizePercentageValue(
       int note, int compPart, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   [[nodiscard]] const musicDevice::description::sound::Parameter*
   parameterDescription(int compPart, int parameterIdx) const;

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<CompositeSound>;
   VoiceContainer& voices() noexcept;
   friend auto meta::registerMembers<MelodicInstrument>();
   friend bool isSameInstrument(const MelodicInstrument& lhs,
                                const MelodicInstrument& rhs);

   template <typename Cb> void forEachVoice(Cb&& cb);

private:
   VoiceContainer m_voices;
   std::string m_name;
   std::shared_ptr<RtData> m_pRtData;
};

}   // namespace instruments
}   // namespace base

#include "MelodicInstrument.inl"
#include "MelodicInstrumentMeta.h"

#endif   // MELODIC_INSTRUMENT_H