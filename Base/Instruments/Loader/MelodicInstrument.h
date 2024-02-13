#ifndef MELODIC_INSTRUMENT_LOADER_H
#define MELODIC_INSTRUMENT_LOADER_H

#include <array>
#include <memory>
#include <vector>

#include "Instrument.h"
#include "MusicDeviceId.h"
#include "SoundSection.h"
#include "ParameterData.h"

class MusicDevice;

namespace base::instruments::loader
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


   friend bool isSameInstrument(const MelodicInstrument& lhs,
                                const MelodicInstrument& rhs);
   friend class MelodicInstrumentsModifier;
   // template <typename Cb> void forEachComponent(Cb&& cb);
   // template <typename Cb> void forEachComponent(size_t componentIdx, Cb&& cb);
   // template <typename Cb> void forEachComponentExt(Cb&& cb);
   // template <typename Cb> void forEachLeadComponent(Cb&& cb);
   // template <typename Cb> void forEachLeadComponent(Cb&& cb) const;
   // template <typename Cb> void forEachLeadComponentExt(Cb&& cb) const;

   static constexpr int NUM_COMPONENTS = 4;
   struct Voice 
   {
      struct Component
      {
         musicDevice::MusicDeviceId soundDeviceId;
         int sdVoiceIdx{0};
      };
      std::array<std::optional<Component>, NUM_COMPONENTS> components;
   };
   struct ParameterData {
      musicDevice::description::sound::Engine* pDescription{nullptr}; // TODO: maybe music device type would be enough
      std::vector<musicDevice::sound::ParameterData> deviceParameters;
      int noteOffset{0};
      float amp{1.0f};
   };      

private:
   std::string m_name;
   std::vector<Voice> m_voices;
   std::array<std::optional<ParameterData>, NUM_COMPONENTS> m_parameters;

   friend auto meta::registerMembers<MelodicInstrument>();
   friend auto meta::registerMembers<MelodicInstrument::ParameterData>();
   friend auto meta::registerMembers<MelodicInstrument::Voice>();
   friend auto meta::registerMembers<MelodicInstrument::Voice::Component>();
};

}   // namespace base::instruments::loader

#include "MelodicInstrument.inl"
#include "MelodicInstrumentMeta.h"

#endif   // MELODIC_INSTRUMENT_H
