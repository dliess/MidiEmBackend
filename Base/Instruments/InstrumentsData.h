#ifndef INSTRUMENTS_DATA_H
#define INSTRUMENTS_DATA_H

#include <vector>

#include "KitInstrument.h"
#include "MelodicInstrument.h"

namespace base::instruments
{
    
using KitInstruments     = std::vector<KitInstrument>;
using MelodicInstruments = std::vector<MelodicInstrument>;
struct Data
{
   KitInstruments kitInstruments;
   MelodicInstruments melodicInstruments;
};

}   // namespace base::instruments

#include "InstrumentsDataMeta.h"

#endif