#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_META_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "InstrumentComponent.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::Component>()
{
   return members(member("soundDeviceId", &base::instruments::Component::m_soundDeviceId),
                  member("sdVoiceIdx", &base::instruments::Component::m_sdVoiceIdx),
                  member("noteOffset", &base::instruments::Component::m_noteOffset));
}

} // namespace meta

#endif