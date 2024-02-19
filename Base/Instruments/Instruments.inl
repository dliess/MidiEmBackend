#include "Identifiable.h"
#include "Instruments.h"

namespace base::instruments
{

/*
inline void Instruments::withInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const Instrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.kitInstruments, uuid, cb);
      util::withUuid(rtData.melodicInstruments, uuid, cb);
   });
}

inline void Instruments::withKitInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const KitInstrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.kitInstruments, uuid, cb);
   });
}

inline void Instruments::withMelodicInstrumentRt(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const MelodicInstrument&)> cb)
{
   m_doubleBufferedData.withRtLocked([uuid, cb](const auto& rtData) {
      util::withUuid(rtData.melodicInstruments, uuid, cb);
   });
}
*/

}   // namespace base::instruments
