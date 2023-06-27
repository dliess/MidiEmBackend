#ifndef MELODIC_INSTRUMENT_H
#define MELODIC_INSTRUMENT_H

#include <array>
#include <memory>
#include <vector>

#include "Instrument.h"
#include "MelodicInstrumentVoice.h"
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
   void pitchBendMPE(int note, float value) const;
   void incrementParameterValue(int componentIdx, int parameterId,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   void incrementParameterValue(int note, int componentIdx, int parameterId,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, bool roundRobin) const;
   [[nodiscard]] std::optional<float> getParameterValue(
       int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] std::optional<float> getParameterValue(
       int note, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int componentIdx, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   void setParameterValueMPE(int note, int componentIdx, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;
   [[nodiscard]] float fromNormalizedValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   [[nodiscard]] float fromNormalizedValue(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const;
   [[nodiscard]] const musicDevice::description::sound::Parameter*
        parameterDescription(int componentIdx, int parameterIdx) const;

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<MelodicVoice>;
   VoiceContainer& voices() noexcept;
   const VoiceContainer& voices() const noexcept;

   void updateParameterUI() const;

   friend void to_json(nlohmann::json& j, const MelodicInstrument& instr);
   friend void from_json(const nlohmann::json& j, MelodicInstrument& instr);

   friend bool isSameInstrument(const MelodicInstrument& lhs,
                                const MelodicInstrument& rhs);

   template <typename Cb> void forEachComponent(Cb&& cb);
   template <typename Cb> void forEachComponent(size_t componentIdx, Cb&& cb);
   template <typename Cb> void forEachComponentExt(Cb&& cb);
   template <typename Cb> void forEachLeadComponent(Cb&& cb);
   template <typename Cb> void forEachLeadComponent(Cb&& cb) const;
   template <typename Cb> void forEachLeadComponentExt(Cb&& cb) const;

   [[nodiscard]] const Component* getFirstComponent(size_t componentIdx) const;

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