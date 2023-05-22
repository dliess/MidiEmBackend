#include "Identifiable.h"
#include "Instruments.h"

namespace base::instruments
{
inline void Instruments::withInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const Instrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.kitInstruments.cbegin(),
                     rtData.kitInstruments.cend(), uuid, cb);
      util::withUuid(rtData.melodicInstruments.cbegin(),
                     rtData.melodicInstruments.cend(), uuid, cb);
   });
}

inline void Instruments::withKitInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const KitInstrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.kitInstruments.cbegin(),
                     rtData.kitInstruments.cend(), uuid, cb);
   });
}

inline void Instruments::withMelodicInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const MelodicInstrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.melodicInstruments.cbegin(),
                     rtData.melodicInstruments.cend(), uuid, cb);
   });
}

inline bool Instruments::hasKitInstrument(
    util::Identifiable::UUIDView uuid) const
{
   const auto it = std::ranges::find_if(
       m_doubleBufferedData.nonRt().kitInstruments,
       [&uuid](const KitInstrument& instr) { return uuid == instr.idView(); });
   return it != m_doubleBufferedData.nonRt().kitInstruments.end();
}

inline bool Instruments::hasMelodicInstrument(
    util::Identifiable::UUIDView uuid) const
{
   const auto it =
       std::ranges::find_if(m_doubleBufferedData.nonRt().melodicInstruments,
                            [&uuid](const MelodicInstrument& instr) {
                               return uuid == instr.idView();
                            });
   return it != m_doubleBufferedData.nonRt().melodicInstruments.end();
}

inline void Instruments::updateParameterUI()
{
   m_doubleBufferedData.withRtLocked([](const auto& rtData) {
      for(const auto& instr : rtData.kitInstruments) 
      { 
         instr.updateParameterUI();
      }
      for(const auto& instr : rtData.melodicInstruments) 
      { 
         instr.updateParameterUI();
      }
   });
}

}   // namespace base::instruments