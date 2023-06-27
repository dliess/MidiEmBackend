#ifndef UTIL_PARAMETER_H
#define UTIL_PARAMETER_H

#include "StrongType.h"
#include <mpark/variant.hpp>

template <typename Tag>
using FloatingPointType =
    util::StrongType<float, Tag, util::Addable, util::Substractable,
                     util::Multipliable, util::Dividable, util::Printable,
                     util::Swappable>;

using ListRangeEnd = util::StrongType<std::size_t, struct ListRangeType>;
using FloatingPointRangeEnd = util::StrongType<float, struct FloatingPointRangeType>;
using ValueRangeEnd = mpark::variant<FloatingPointRangeEnd, ListRangeEnd>;

#endif