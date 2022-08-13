#ifndef KIT_INSTRUMENT_META_H
#define KIT_INSTRUMENT_META_H

#include "Meta.h"

#include <JsonCast.h>
namespace base::instruments{
#include "JsonCastNamespaceFix.h"
}

namespace meta
{
template<>
inline auto registerMembers<base::instruments::KitInstrument>()
{
   return members(
      member("id", &base::instruments::KitInstrument::m_uuid),
      member("name", &base::instruments::KitInstrument::m_name),
      member("sounds", &base::instruments::KitInstrument::m_compositeSounds)
   );
}

template<>
inline auto registerMembers<base::instruments::Voice>()
{
   return members(member("soundDeviceId", &base::instruments::Voice::soundDeviceId),
                  member("voiceIndex", &base::instruments::Voice::voiceIndex),
                  member("noteOffset", &base::instruments::Voice::noteOffset));
}

template<>
inline auto registerMembers<base::instruments::CompositeSound>()
{
   return members(member("voices", &base::instruments::CompositeSound::voices));
}

} // namespace meta

#endif // KIT_INSTRUMENT_META_H