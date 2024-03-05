#ifndef MELODIC_INSTRUMENT_COPYER_H
#define MELODIC_INSTRUMENT_COPYER_H

#include "Loader/MelodicInstrument.h"
#include "Rt/MelodicInstrument.h"
#include "MusicDeviceContainer.h"

namespace base::instruments
{


class MelodicInstrumentCopyer
{
public:
   static rt::MelodicInstrument copy(const base::musicDevice::MusicDeviceContainer& mdContainer,
                                     const loader::MelodicInstrument& src) noexcept;
};

}   // namespace base::instruments


#endif // MELODIC_INSTRUMENT_COPYER_H
