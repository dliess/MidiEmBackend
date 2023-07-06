#ifndef SYSEX_DESCRIPTION_H
#define SYSEX_DESCRIPTION_H

#include <vector>
#include <cstdint>

namespace base::musicDevice::description::midisysex
{
struct OffsetCache
{
   static constexpr int UNSET = -1;
   int offset{UNSET};   // For caching, do NOT REFLECT !!!!
};

struct Field : public OffsetCache
{
   constexpr int sizeInSysex() const noexcept { return 1; }
};

// We need this as a workaround because every struct has
// to contain an element to work with json-stuff
struct FieldWithSize : public OffsetCache
{
   int size;
   constexpr int sizeInSysex() const noexcept { return size; }
};

struct Bytes : public Field
{
   std::vector<uint8_t> values;
   int sizeInSysex() const noexcept { return values.size(); }
};

}   // namespace base::musicDevice::description::midisysex

#include "SysExDescriptionMeta.h"

#endif