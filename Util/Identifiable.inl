#ifndef UTIL_IDENTIFIABLE_INL
#define UTIL_IDENTIFIABLE_INL

#include "Identifiable.h"

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

} // namespace util

#endif