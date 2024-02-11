#ifndef MELODIC_INSTRUMENTS_INSTRUMENT_COMPONENT_META_H
#define MELODIC_INSTRUMENTS_INSTRUMENT_COMPONENT_META_H

#include "Meta.h"
#include <JsonCast.h>

#include "MelodicInstrumentComponent.h"

namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::MelodicComponent>()
{
   return members(member("parameterData", &base::instruments::loader::MelodicComponent::m_parameterData),
                  member("soundDeviceId", &base::instruments::loader::MelodicComponent::m_soundDeviceId),
                  member("sdVoiceIdx", &base::instruments::loader::MelodicComponent::m_sdVoiceIdx),
                  member("noteOffset", &base::instruments::loader::MelodicComponent::m_noteOffset)
   );
}

} // namespace meta

#endif
