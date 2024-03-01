#ifndef INSTRUMENTS_KIT_INSTRUMENTS_VOICE_RT_H
#define INSTRUMENTS_KIT_INSTRUMENTS_VOICE_RT_H

#include <string>

#include "Identifiable.h"
#include "KitInstrumentComponent.h"
#include <vector>

namespace base::instruments::rt
{
struct KitVoice   //: public util::Identifiable
{
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   std::vector<KitComponent> components;
   int noteOffset{0};
   float amp{1.0f};
};

}   // namespace base::instruments::rt

#endif
