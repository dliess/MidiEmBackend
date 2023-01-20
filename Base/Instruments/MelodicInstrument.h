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

   MelodicInstrument() = default;
   MelodicInstrument(std::string name, std::shared_ptr<RtData> rtData) noexcept;
   MelodicInstrument(const MelodicInstrument&) noexcept = delete;
   MelodicInstrument& operator=(const MelodicInstrument&) noexcept = delete;
   MelodicInstrument(MelodicInstrument&&) noexcept                 = default;
   MelodicInstrument& operator=(MelodicInstrument&&) noexcept = default;

   void noteOn(int note, float velocity) const noexcept override;
   void noteOff(int note, float velocity) const noexcept override;

   void pitchBend(float value) const noexcept;
   void incrementParameterValue(int compPart, int parameterId, float increment,
                                bool roundRobin = false) const noexcept;
   void incrementParameterValue(int note, int compPart, int parameterId,
                                float increment,
                                bool roundRobin = false) const noexcept;
   [[nodiscard]] float getParameterValue(
       int compPart, int parameterIdx,
       musicDevice::sound::ParameterPart parameterPart =
           musicDevice::sound::ParameterPart::Commanded) const noexcept;
   [[nodiscard]] float getParameterValue(
       int note, int compPart, int parameterIdx,
       musicDevice::sound::ParameterPart parameterPart =
           musicDevice::sound::ParameterPart::Commanded) const noexcept;
   void setParameterValue(int compPart, int parameterId,
                          float value) const noexcept;
   void setParameterValue(int note, int compPart, int parameterId,
                          float value) const noexcept;
   [[nodiscard]] float normalizePercentageValue(
       int compPart, int parameterId,
       musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const noexcept;
   [[nodiscard]] float normalizePercentageValue(
       int note, int compPart, int parameterId,
       musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const noexcept;
   [[nodiscard]] const musicDevice::description::sound::Parameter*
   parameterDescription(int compPart, int parameterIdx) const;

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<CompositeSound>;
   VoiceContainer& voices() noexcept;
   friend auto meta::registerMembers<MelodicInstrument>();

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