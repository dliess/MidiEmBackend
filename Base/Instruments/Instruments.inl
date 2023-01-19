#include "Identifiable.h"
#include "Instruments.h"

namespace base::instruments
{
inline void Instruments::withKitInstrument(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const KitInstrument&)> cb)
{
   data.withRtLocked([&uuid, &cb](const auto& rtData) {
      util::withUuid(rtData.kitInstruments.cbegin(), rtData.kitInstruments.cend(),
                     uuid, cb);
   });
}

inline void Instruments::withMelodicInstrument(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(const MelodicInstrument&)> cb)
{
   data.withRtLocked([&uuid, &cb](const auto& rtData) {
      util::withUuid(rtData.melodicInstruments.cbegin(),
                     rtData.melodicInstruments.cend(), uuid, cb);
   });
}
}   // namespace base::instruments