#ifndef UTIL_REPLACE_ASTERISK_TO_LOCALHOST_H
#define UTIL_REPLACE_ASTERISK_TO_LOCALHOST_H

#include <string>

namespace util
{
inline
std::string replaceAsteriskToLocalhost(const std::string& str)
{
   const auto pos = str.find("*");
   if (pos == std::string::npos)
   {
      return str;
   }
   return std::string(str).replace(pos, 1, "localhost");
}

}   // namespace util

#endif