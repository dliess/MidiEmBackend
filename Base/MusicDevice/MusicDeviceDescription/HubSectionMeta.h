#ifndef DESCRIPTOR_HUB_SECTION_META_H
#define DESCRIPTOR_HUB_SECTION_META_H

#include "HubSection.h"
#include "Meta.h"

namespace meta
{
template<>
inline auto registerMembers<base::musicDevice::description::hub::Section>()
{
   return members(
      member("hasInput", &base::musicDevice::description::hub::Section::hasInput),
      member("hasOutput", &base::musicDevice::description::hub::Section::hasOutput)
   );
}

} // namespace meta


#endif