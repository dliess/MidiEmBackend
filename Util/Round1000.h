#ifndef UTIL_ROUND1000_H
#define UTIL_ROUND1000_H

namespace util
{
constexpr double round1000(double num) { return round(num * 1000) / 1000.0; }

}   // namespace util

#endif