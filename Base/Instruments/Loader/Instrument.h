#ifndef BASE_INSTRUMENT_LOADER_H
#define BASE_INSTRUMENT_LOADER_H

#include "Identifiable.h"

namespace base::instruments::loader
{

class Instrument : public util::Identifiable
{
public:
   void markAsDefaultCreated() noexcept { m_isDefaultCreated = true; };
   void unmarkAsDefaultCreated() noexcept { m_isDefaultCreated = false; };
   bool isDefaultCreated() const noexcept { return m_isDefaultCreated; }
   int refCount() const { return m_refCount; }
   void incRefCount() { ++m_refCount; }
   void decRefCount() { if(m_refCount > 0) --m_refCount; }
protected:
   bool m_isDefaultCreated{false};
   int m_refCount{0};
};

} // namespace base::instruments::loader
#endif
