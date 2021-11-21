#ifndef BASE_ARP_TYPES_H
#define BASE_ARP_TYPES_H

#include "EnumReflect.h"

namespace base::arp
{
DECLARE_ENUM(Algorithm, uint, Up, Down, UpDown, Random, RecvOrder,
             CustomSequence);
DECLARE_ENUM(RangeType, uint, Octave, Notes);

}   // namespace base::arp

#endif