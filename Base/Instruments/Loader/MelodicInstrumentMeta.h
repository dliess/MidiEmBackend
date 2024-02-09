#ifndef MELODIC_INSTRUMENT_LOADER_META_H
#define MELODIC_INSTRUMENT_LOADER_META_H

#include <nlohmann/json.hpp>

#include <Meta.h>
#include <JsonCast.h>

namespace meta {

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument>()
{
    return members(member("id", &base::instruments::loader::MelodicInstrument::m_uuid),
                   member("name", &base::instruments::loader::MelodicInstrument::m_name),
                   member("voices", &base::instruments::loader::MelodicInstrument::m_voices));
}

} // namespace meta

#endif // MELODIC_INSTRUMENT_LOADER_META_H
