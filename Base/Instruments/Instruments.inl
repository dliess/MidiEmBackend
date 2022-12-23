#include "Instruments.h"

namespace base::instruments {

template<class CB>
void Instruments::withKitInstrument(util::Identifiable::UUIDView uuid, CB&& cb)
{
   auto it = std::find_if(
       data.kitInstruments.begin(),
       data.kitInstruments.end(),
       [&uuid](const auto& instr) { return instr.id() == uuid; });
   if(it != data.kitInstruments.end())
   {
      cb(*it);
   }
}

template<class CB>
void Instruments::withMelodicInstrument(util::Identifiable::UUIDView uuid, CB&& cb)
{
   auto it = std::find_if(
       data.melodicInstruments.begin(),
       data.melodicInstruments.end(),
       [&uuid](const auto& instr) { return instr.id() == uuid; });
   if(it != data.melodicInstruments.end())
   {
      cb(*it);
   }
}

}