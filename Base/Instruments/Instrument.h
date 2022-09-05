#ifndef BASE_INSTRUMENT_H
#define BASE_INSTRUMENT_H

#include "Identifiable.h"
#include "NotePlayer.h"
namespace base::instruments
{
class Instrument : public util::Identifiable, public INotePlayer
{
public:
   void markAsDefaultCreated() noexcept { m_isDefaultCreated = true; };
   bool isDefaultCreated() const noexcept { return m_isDefaultCreated; }
protected:
   bool m_isDefaultCreated{false};
};

} // namespace base::instruments
#endif