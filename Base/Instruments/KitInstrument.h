#ifndef KIT_INSTRUMENT_H
#define KIT_INSTRUMENT_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "CompositeSound.h"
#include "Instrument.h"
#include "MusicDeviceId.h"
#include "function_ref.h"

namespace base
{
namespace musicDevice
{
struct MusicDevice;
struct Holder;
}   // namespace musicDevice

namespace instruments
{
class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   KitInstrument(std::string name) noexcept;
   void noteOn(int note, float velocity, void* token = nullptr) const override;
   void noteOff(int note, float velocity, void* token = nullptr) const override;
   void noteOn(int soundIndex, int note, float velocity) const;
   void noteOff(int soundIndex, int note, float velocity) const;
   void incrementParameterValue(int soundIdx, int componentIdx,
                                int parameterIdx, float increment,
                                bool roundRobin = false) const;
   [[nodiscard]] float getParameterValue(
       int soundIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterPart parameterPart =
           musicDevice::sound::ParameterPart::Commanded) const;
   void setParameterValue(int soundIdx, int componentIdx, int parameterIdx,
                          float value) const;
   [[nodiscard]] float normalizePercentageValue(
       int soundIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterPart parameterPart,
       float percentageValue) const;

   [[nodiscard]] const musicDevice::description::sound::Parameter*
   parameterDescription(int soundIdx, int componentIdx, int parameterIdx) const;

   template <typename T> void addSound(int padIdx, T&& kompositeSound);

   inline std::string name() const noexcept;
   inline void setName(const std::string& name) noexcept;

   inline std::vector<CompositeSound>& sounds() noexcept;
   inline const std::vector<CompositeSound>& sounds() const noexcept;

   template <typename Cb> void forEachVoice(Cb&& cb);

   friend auto meta::registerMembers<KitInstrument>();
   friend bool isSameInstrument(const KitInstrument& lhs,
                                const KitInstrument& rhs);

private:
   std::string m_name;
   std::vector<CompositeSound> m_compositeSounds;
   std::optional<int> toSoundIndex(int note) const;
   inline void withVoice(int soundIdx, int componentIdx,
                         util::function_ref<void(const Voice&)> cb) const;
};

 
}   // namespace instruments
}   // namespace base

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H