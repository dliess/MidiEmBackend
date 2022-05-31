#ifndef UTIL_IDENTIFIABLE_H
#define UTIL_IDENTIFIABLE_H

#include <uuid/uuid.h>
#include <string.h>
#include <array>
#include <cstdint>
#include <string>

#include "arrayCount.h"

namespace util
{

class Identifiable
{
protected:
   inline Identifiable() noexcept;
   inline Identifiable(const Identifiable& other) noexcept;
   inline Identifiable& operator=(const Identifiable& other) noexcept;
   inline Identifiable(Identifiable&& other) noexcept = default;
   inline Identifiable& operator=(Identifiable&& other) noexcept = default;
public:
   using UUID = std::array<uint8_t, util::array_count_v<uuid_t>>;
   inline const UUID &id() const noexcept;
protected:
   UUID m_uuid;
};

inline std::string uuid2Str(const Identifiable::UUID& uuid);

} // namespace util

inline bool operator<(const util::Identifiable::UUID& lhs, const util::Identifiable::UUID& rhs) noexcept
{
   for(int i = 0; i <lhs.size(); ++i)
   {
      if(lhs[i] != rhs[i]) return lhs[i] < rhs[i];
   }
   return false;
}

namespace std
{
  template <>//
  struct hash<util::Identifiable::UUID>
  {
      size_t operator()(const util::Identifiable::UUID& uuid) const noexcept
      {
         return uuid[0] + uuid[7] + uuid[15];
      }
  };
} // namespace std

#include "Identifiable.inl"
//#include "IdentifiableMeta.h"

#endif