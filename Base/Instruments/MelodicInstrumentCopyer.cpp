#include "MelodicInstrumentCopyer.h"
#include "Rt/MelodicInstrument.h"

using namespace base::instruments;


rt::MelodicInstrument MelodicInstrumentCopyer::copy(const loader::MelodicInstrument& src) noexcept
{
   rt::MelodicInstrument dst(src.id());
   for (const auto& voice : src.m_voices)
   {
      //dst.addVoice(voice);
   }
   return dst;
}



