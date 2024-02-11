#ifndef MELODIC_INSTRUMENT_LOADER_H
#define MELODIC_INSTRUMENT_LOADER_H

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
namespace instruments::loader
{
class MelodicInstrument : public Instrument
{
public:

   MelodicInstrument() = default;
   MelodicInstrument(std::string name) noexcept;

   void setParameterValue(int componentIdx, int parameterId,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);

   [[nodiscard]] const musicDevice::description::sound::Parameter*
   parameterDescription(int componentIdx, int parameterIdx) const;

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<MelodicVoice>;
   using ParameterData = musicDevice::sound::ParameterData;
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   using ParameterContainer = std::array<std::vector<ParameterData>, NUM_MAX_COMPONENTS_PER_VOICE>;
   VoiceContainer& voices() noexcept;
   const VoiceContainer& voices() const noexcept;

   friend auto meta::registerMembers<MelodicInstrument>();
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
   ParameterContainer m_parameters;
   std::string m_name;
};

}   // namespace instruments::loader
}   // namespace base

#include "MelodicInstrument.inl"
#include "MelodicInstrumentMeta.h"

#endif   // MELODIC_INSTRUMENT_H
