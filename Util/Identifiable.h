#ifndef UTIL_IDENTIFIABLE_H
#define UTIL_IDENTIFIABLE_H

#include <string.h>
#include <uuid/uuid.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <algorithm>
#include <optional>
#include <type_traits>

#include "arrayCount.h"

namespace util
{
class Identifiable
{
protected:
   inline Identifiable() noexcept;
public:
   using UUID     = std::array<uint8_t, util::array_count_v<uuid_t>>;
   using UUIDView = std::span<const uint8_t, util::array_count_v<uuid_t>>;
   inline const UUID& id() const noexcept;
   inline UUIDView idView() const noexcept;

protected:
   UUID m_uuid;
};

inline std::string uuid2Str(const Identifiable::UUID& uuid);
inline std::string uuid2Str(Identifiable::UUIDView uuid);

template <class Container, class Callable>
void withUuid(const Container& container, Identifiable::UUIDView uuid,
              Callable&& cb)
{
   auto it = std::ranges::find_if(container,
                          [uuid](const auto& e) { return e.idView() == uuid; });
   if (it != container.end())
   {
      std::forward<Callable>(cb)(*it);
   }
}

template <class Container, class Callable>
auto withUuidRet(const Container& container, Identifiable::UUIDView uuid,
              Callable&& cb) -> std::optional< std::invoke_result_t<Callable, typename Container::value_type> >
{
   auto it = std::ranges::find_if(container,
                          [uuid](const auto& e) { return e.idView() == uuid; });
   if (it != container.end())
   {
      return std::forward<Callable>(cb)(*it);
   }
   return std::nullopt;
}

struct IdentifiableHash
{
   using is_transparent = void;
   [[nodiscard]] size_t operator()(
       const Identifiable::UUID& uuid) const noexcept
   {
      return uuid[0] + uuid[7] + uuid[15];
   }
   [[nodiscard]] size_t operator()(Identifiable::UUIDView uuid) const noexcept
   {
      return uuid[0] + uuid[7] + uuid[15];
   }
};

inline auto deepCopy(Identifiable::UUIDView uuidView)
{
   Identifiable::UUID ret;
   std::copy(uuidView.begin(), uuidView.end(), ret.begin());
   return ret;
}

}   // namespace util

inline bool operator<(const util::Identifiable::UUID& lhs,
                      const util::Identifiable::UUID& rhs) noexcept
{
   for (int i = 0; i < lhs.size(); ++i)
   {
      if (lhs[i] != rhs[i])
         return lhs[i] < rhs[i];
   }
   return false;
}

namespace std
{
inline bool operator==(util::Identifiable::UUIDView lhs,
                       util::Identifiable::UUIDView rhs) noexcept
{
   return std::equal(begin(lhs), end(lhs), begin(rhs), end(rhs));
}

inline bool operator!=(util::Identifiable::UUIDView lhs,
                       util::Identifiable::UUIDView rhs) noexcept
{
   return !(lhs == rhs);
}
}   // namespace std

#include "Identifiable.inl"
//#include "IdentifiableMeta.h"

#endif