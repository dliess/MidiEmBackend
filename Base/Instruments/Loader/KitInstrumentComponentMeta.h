#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_META_H
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "KitInstrumentComponent.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::KitComponent>()
{
   return members(member("parameterData", &base::instruments::loader::KitComponent::m_parameterData),
                  member("soundDeviceId", &base::instruments::loader::KitComponent::m_soundDeviceId),
                  member("sdVoiceIdx", &base::instruments::loader::KitComponent::m_sdVoiceIdx),
                  member("noteOffset", &base::instruments::loader::KitComponent::m_noteOffset)
   );
}

} // namespace meta

#endif
