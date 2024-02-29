#include "KitInstrumentCopyer.h"
#include "Rt/KitInstrument.h"

using namespace base::instruments;


rt::KitInstrument KitInstrumentCopyer::copy(const loader::KitInstrument& src) noexcept
{
   rt::KitInstrument dst(src.id(), src.name());
   for (const auto& voice : src.m_voices)
   {
      //dst.addVoice(voice);
   }
   return dst;
}



