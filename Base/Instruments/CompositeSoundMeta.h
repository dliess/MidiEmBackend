#ifndef INSTRUMENTS_COMPOSITE_SOUND_META_H
#define INSTRUMENTS_COMPOSITE_SOUND_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "CompositeSound.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::CompositeSound>()
{
   return members(member("voices", &base::instruments::CompositeSound::voices));
}

} // namespace meta

#endif