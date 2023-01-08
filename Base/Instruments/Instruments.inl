#include "Identifiable.h"
#include "Instruments.h"

namespace base::instruments
{
inline void Instruments::withKitInstrument(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(KitInstrument&)> cb)
{
   util::withUuid(data.kitInstruments.begin(), data.kitInstruments.end(), uuid,
                  cb);
}

inline void Instruments::withMelodicInstrument(
    util::Identifiable::UUIDView uuid,
    util::function_ref<void(MelodicInstrument&)> cb)
{
   util::withUuid(data.melodicInstruments.begin(),
                  data.melodicInstruments.end(), uuid, cb);
}
}   // namespace base::instruments