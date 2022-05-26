#ifndef UTIL_IDENTIFIABLE_INL
#define UTIL_IDENTIFIABLE_INL

#include "Identifiable.h"
#include <sstream>

namespace util
{
inline Identifiable::Identifiable() noexcept { uuid_generate(m_uuid.data()); };
inline Identifiable::Identifiable(const Identifiable& other) noexcept
{
   uuid_generate(m_uuid.data());
}
inline Identifiable& Identifiable::operator=(const Identifiable& other) noexcept
{
   uuid_generate(m_uuid.data());
   return *this;
}

inline const Identifiable::UUID& Identifiable::id() const noexcept
{
   return m_uuid;
}

inline std::string Identifiable::uuid2Str() const
{
   std::stringstream ss;
   for(const auto& e : m_uuid)
   {
      ss << int(e) << ":"; 
   }
   return ss.str();
}

} // namespace util

#endif