#ifndef MELODIC_INSTRUMENT_LOADER_H
#define MELODIC_INSTRUMENT_LOADER_H

#include <array>
#include <memory>
#include <vector>

#include "Instrument.h"
#include "MusicDeviceId.h"
#include "SoundSection.h"
#include "ParameterData.h"

namespace base::instruments { class MelodicInstrumentCopyer; }

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

   friend bool isSameInstrument(const MelodicInstrument& lhs,
                                const MelodicInstrument& rhs);
   friend class MelodicInstrumentsModifier;
   friend class MelodicInstrumentModifier;

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
      struct EngineId {
         musicDevice::MusicDeviceName mdName;
         int engineIdx{0};
         bool operator==(const EngineId& rhs) const = default;
      };
      using ParametersDescr = std::vector<musicDevice::description::sound::Parameter>;
      explicit ParameterData(EngineId engineId, const ParametersDescr& parametersDescr, size_t numParameters) : 
            engineId(std::move(engineId)), 
            parametersDescr(&parametersDescr), 
            deviceParameters(numParameters) {}
      ParameterData() = default;
      EngineId engineId;
      const ParametersDescr* parametersDescr{nullptr}; // TODO: maybe music device type would be enough
      std::vector<musicDevice::sound::ParameterData> deviceParameters;
      int noteOffset{0};
      float amp{1.0f};
   };      

private:
   // std::string m_name;
   std::vector<Voice> m_voices;
   std::array<std::optional<ParameterData>, NUM_COMPONENTS> m_parameters;

   friend auto meta::registerMembers<MelodicInstrument>();
   friend auto meta::registerMembers<MelodicInstrument::ParameterData>();
   friend auto meta::registerMembers<MelodicInstrument::Voice>();
   friend auto meta::registerMembers<MelodicInstrument::Voice::Component>();
   friend class base::instruments::MelodicInstrumentCopyer;
};

}   // namespace base::instruments::loader

#include "MelodicInstrument.inl"
#include "MelodicInstrumentMeta.h"

#endif   // MELODIC_INSTRUMENT_H
