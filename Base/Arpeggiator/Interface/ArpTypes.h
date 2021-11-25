#ifndef BASE_ARP_TYPES_H
#define BASE_ARP_TYPES_H

#include "EnumReflect.h"

namespace base::arp
{
DECLARE_ENUM(FeedMode, uint, Control, Sequence);
DECLARE_ENUM(Algorithm, uint, Up, Down, UpDown, Random, RecvOrder);
DECLARE_ENUM(RangeType, uint, Octave, Notes);

}   // namespace base::arp

#endif