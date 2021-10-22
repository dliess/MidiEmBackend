
#ifndef _ENUM_REFLECT_H_
#define _ENUM_REFLECT_H_

#include <map>
#include <ostream>
#include <regex>
#include <string>
#include <type_traits>

namespace util
{
template <typename E>
std::map<E, std::string> reflect_enum(const char *enumsInfo)
{
   std::map<E, std::string> enum2string;

   if ((enumsInfo == nullptr) || (*enumsInfo == 0))
   {
      return enum2string;
   }

   // Should be called once per each enumeration.
   std::string senumsInfo(enumsInfo);
   std::regex re(
       "^([a-zA-Z_][a-zA-Z0-9_]+) *=? *([^,]*)(,|$) *");   // C++ identifier to
                                                           // optional " =
                                                           // <value>"
   std::smatch sm;

   for (int value  = 0; regex_search(senumsInfo, sm, re);
        senumsInfo = sm.suffix(), value++)
   {
      std::string enumName  = sm[1].str();
      std::string enumValue = sm[2].str();
      if (enumValue.length() != 0)
      {
         if (enumValue.compare(0, 2, "0b") == 0)
         {
            value = std::stoi(enumValue.substr(2), nullptr, 2);
         }
         else
         {
            value = std::stoi(enumValue, nullptr, 0);
         }
      }
      E enumEntry            = static_cast<E>(value);
      enum2string[enumEntry] = enumName;
   }
   return enum2string;
}

}   // namespace util

#define DECLARE_ENUM(EName, basetype, ...)                            \
   enum class EName : basetype                                        \
   {                                                                  \
      __VA_ARGS__                                                     \
   };                                                                 \
   static std::map<EName, std::string> E_Map_##EName(                 \
       util::reflect_enum<EName>(#__VA_ARGS__));                      \
                                                                      \
   static std::ostream &operator<<(std::ostream &os, EName e)         \
   {                                                                  \
      os << E_Map_##EName[e];                                         \
      return os;                                                      \
   }                                                                  \
   static std::string operator~(EName e) { return E_Map_##EName[e]; } \
   static std::string operator+(std::string &&str, EName e)           \
   {                                                                  \
      return str + E_Map_##EName[e];                                  \
   }                                                                  \
   static std::string operator+(EName e, std::string &&str)           \
   {                                                                  \
      return E_Map_##EName[e] + str;                                  \
   }                                                                  \
   static std::string &operator+=(std::string &str, EName e)          \
   {                                                                  \
      str += E_Map_##EName[e];                                        \
      return str;                                                     \
   }                                                                  \
   inline                                                             \
   EName create_##EName(const std::string& enumStr)                   \
   {                                                                  \
      auto it = std::find_if(E_Map_##EName.begin(), E_Map_##EName.end(),  \
         [&enumStr](std::pair<EName, std::string> const& item)            \
            { return item.second == enumStr; });                          \
      if(it == E_Map_##EName.end()) return E_Map_##EName.begin()->first;  \
      return it->first;                                                   \
   }


#endif /* _ENUM_REFLECT_H_ */