#ifndef INSTRUMENT_VOICE_META_H
#define INSTRUMENT_VOICE_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "InstrumentVoice.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::Voice>()
{
   return members(member("components", &base::instruments::Voice::components));
}

} // namespace meta

#endif