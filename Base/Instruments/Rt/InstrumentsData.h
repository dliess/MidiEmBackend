#ifndef INSTRUMENTS_DATA_RT_H
#define INSTRUMENTS_DATA_RT_H

#include <vector>

#include "KitInstrument.h"
#include "MelodicInstrument.h"

namespace base::instruments::rt
{
    
using KitInstruments     = std::vector<KitInstrument>;
using MelodicInstruments = std::vector<MelodicInstrument>;
struct Data
{
   KitInstruments kitInstruments;
   MelodicInstruments melodicInstruments;
};

}   // namespace base::instruments::rt

#endif
