#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_META_H
#define INSTRUMENTS_INSTRUMENT_COMPONENT_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "InstrumentComponent.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::Component>()
{
   return members(member("parameterData", &base::instruments::loader::Component::m_parameterData),
                  member("soundDeviceId", &base::instruments::loader::Component::m_soundDeviceId),
                  member("sdVoiceIdx", &base::instruments::loader::Component::m_sdVoiceIdx),
                  member("noteOffset", &base::instruments::loader::Component::m_noteOffset)
   );
}

} // namespace meta

#endif
