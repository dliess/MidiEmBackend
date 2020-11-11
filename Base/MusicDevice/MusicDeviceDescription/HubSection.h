#ifndef DESCRIPTOR_HUB_SECTION_H
#define DESCRIPTOR_HUB_SECTION_H

#include <optional>

namespace base::musicDevice::description::hub
{

struct Section
{
   bool hasInput;
   bool hasOutput;
};

} // namespace base::musicDevice::description::hub

#include "HubSectionMeta.h"

#endif