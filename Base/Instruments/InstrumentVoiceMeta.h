#ifndef INSTRUMENT_VOICE_META_H
#define INSTRUMENT_VOICE_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "InstrumentVoice.h"

namespace base::instruments {
/*
inline
void to_json(nlohmann::json& j, const Voice::Components& data)
{
    
}

inline
void from_json(const nlohmann::json& j, Voice::Components& data)
{
    
}
*/
} // namespace base::instruments


namespace meta
{

template<>
inline auto registerMembers<base::instruments::Voice>()
{
   return members(member("components", &base::instruments::Voice::components));
}

} // namespace meta

#endif