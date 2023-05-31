#ifndef INSTRUMENTS_KIT_INSTRUMENTS_VOICE_META
#define INSTRUMENTS_KIT_INSTRUMENTS_VOICE_META

#include "Meta.h"
#include <JsonCast.h>

#include "KitInstrumentVoice.h"

namespace base::instruments {
/*
inline
void to_json(nlohmann::json& j, const KitVoice::Components& data)
{
    
}

inline
void from_json(const nlohmann::json& j, KitVoice::Components& data)
{
    
}
*/
} // namespace base::instruments


namespace meta
{

template<>
inline auto registerMembers<base::instruments::KitVoice>()
{
   return members(member("components", &base::instruments::KitVoice::components));
}

} // namespace meta

#endif