#ifndef INSTRUMENTS_INSTRUMENT_VOICE_META_H
#define INSTRUMENTS_INSTRUMENT_VOICE_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "InstrumentVoice.h"


namespace base::instruments{
#include "JsonCastNamespaceFix.h"
}


namespace meta
{

template<>
inline auto registerMembers<base::instruments::Voice>()
{
   return members(member("soundDeviceId", &base::instruments::Voice::m_soundDeviceId),
                  member("voiceIndex", &base::instruments::Voice::m_voiceIndex),
                  member("noteOffset", &base::instruments::Voice::m_noteOffset));
}

} // namespace meta

#endif