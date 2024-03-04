#ifndef KIT_INSTRUMENT_META_H
#define KIT_INSTRUMENT_META_H

#include <nlohmann/json.hpp>

#include "Meta.h"
#include <JsonCast.h>
namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::KitInstrument::Component>()
{
   return members(member("parameterData", &base::instruments::loader::KitInstrument::Component::parameterData),
                  member("soundDeviceId", &base::instruments::loader::KitInstrument::Component::soundDeviceId),
                  member("sdVoiceIdx", &base::instruments::loader::KitInstrument::Component::sdVoiceIdx),
                  member("noteOffset", &base::instruments::loader::KitInstrument::Component::noteOffset)
   );
}

template<>
inline auto registerMembers<base::instruments::loader::KitInstrument::Voice>()
{
   return members(
      member("name", &base::instruments::loader::KitInstrument::Voice::name),
      member("components", &base::instruments::loader::KitInstrument::Voice::components),
      member("noteOffset", &base::instruments::loader::KitInstrument::Voice::noteOffset),
      member("amp", &base::instruments::loader::KitInstrument::Voice::amp)
   );
}

template<>
inline auto registerMembers<base::instruments::loader::KitInstrument>()
{
   return members(member("id", &base::instruments::loader::KitInstrument::m_uuid),
                  member("name", &base::instruments::loader::KitInstrument::m_name),
                  member("voices", &base::instruments::loader::KitInstrument::m_voices));
}

} // namespace meta

#endif   // KIT_INSTRUMENT_META_H
