#ifndef UTIL_FLOAT_EQUAL_H
#define UTIL_FLOAT_EQUAL_H

#include <limits>
#include <cmath>

namespace util
{

template <typename T>
T floatEqual(const T& lhs, const T& rhs) {
  return std::fabs(lhs-rhs) < std::numeric_limits<T>::epsilon(); 
}

}   // namespace util

#endif