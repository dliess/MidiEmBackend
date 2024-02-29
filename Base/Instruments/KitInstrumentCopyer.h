#ifndef KIT_INSTRUMENT_COPYER_H
#define KIT_INSTRUMENT_COPYER_H

#include "Loader/KitInstrument.h"
#include "Rt/KitInstrument.h"

namespace base::instruments
{


class KitInstrumentCopyer
{
public:
   static rt::KitInstrument copy(const loader::KitInstrument& src) noexcept;
};

}   // namespace base::instruments


#endif // !KIT_INSTRUMENT_COPYER_H
