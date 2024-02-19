#ifndef BASE_INSTRUMENT_LOADER_H
#define BASE_INSTRUMENT_LOADER_H

#include "Identifiable.h"
#include <string>

namespace base::instruments::loader
{

class Instrument : public util::Identifiable
{
public:
   Instrument() = default;
   Instrument(std::string name) : m_name(std::move(name)) {}
   void markAsDefaultCreated() noexcept { m_isDefaultCreated = true; };
   void unmarkAsDefaultCreated() noexcept { m_isDefaultCreated = false; };
   bool isDefaultCreated() const noexcept { return m_isDefaultCreated; }
   int refCount() const { return m_refCount; }
   void incRefCount() { ++m_refCount; }
   void decRefCount() { if(m_refCount > 0) --m_refCount; }
   std::string name() const noexcept { return m_name; }
   void setName(std::string name) noexcept { m_name = std::move(name); }
protected:
   bool m_isDefaultCreated{false};
   int m_refCount{0};
   std::string m_name;
};

} // namespace base::instruments::loader
#endif
