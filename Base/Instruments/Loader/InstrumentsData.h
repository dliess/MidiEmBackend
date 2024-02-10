#ifndef INSTRUMENTS_DATA_LOADER_H
#define INSTRUMENTS_DATA_LOADER_H

#include <vector>

#include "KitInstrument.h"
#include "MelodicInstrument.h"

namespace base::instruments::loader
{
    
using KitInstruments     = std::vector<KitInstrument>;
using MelodicInstruments = std::vector<MelodicInstrument>;
struct Data
{
   KitInstruments kitInstruments;
   MelodicInstruments melodicInstruments;
};

}   // namespace base::instruments::loader

#include "InstrumentsDataMeta.h"

#endif
