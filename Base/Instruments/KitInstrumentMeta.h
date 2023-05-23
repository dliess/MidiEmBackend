#ifndef KIT_INSTRUMENT_META_H
#define KIT_INSTRUMENT_META_H

#include "Meta.h"

#include <JsonCast.h>

namespace meta
{
template<>
inline auto registerMembers<base::instruments::KitInstrument>()
{
   return members(
      member("id", &base::instruments::KitInstrument::m_uuid),
      member("name", &base::instruments::KitInstrument::m_name),
      member("sounds", &base::instruments::KitInstrument::m_voices)
   );
}

} // namespace meta

#endif // KIT_INSTRUMENT_META_H