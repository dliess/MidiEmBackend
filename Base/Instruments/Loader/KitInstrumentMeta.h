#ifndef KIT_INSTRUMENT_META_H
#define KIT_INSTRUMENT_META_H

#include <nlohmann/json.hpp>

#include "Meta.h"
#include <JsonCast.h>
namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::KitInstrument>()
{
   return members(member("id", &base::instruments::loader::KitInstrument::m_uuid),
                  member("name", &base::instruments::loader::KitInstrument::m_name),
                  member("voices", &base::instruments::loader::KitInstrument::m_voices));
}

} // namespace meta

#endif   // KIT_INSTRUMENT_META_H
