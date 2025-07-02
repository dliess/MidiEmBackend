#ifndef UTIL_CLIP_H
#define UTIL_CLIP_H

#include <algorithm>

namespace util
{

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
  return std::max(lower, std::min(n, upper));
}

}   // namespace util

#endif