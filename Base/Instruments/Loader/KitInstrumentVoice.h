#ifndef INSTRUMENTS_KIT_INSTRUMENTS_VOICE_LOADER_H
#define INSTRUMENTS_KIT_INSTRUMENTS_VOICE_LOADER_H

#include <string>

#include "Identifiable.h"
#include "KitInstrumentComponent.h"
#include <vector>

namespace base::instruments::loader
{
struct KitVoice   //: public util::Identifiable
{
   KitVoice() noexcept = default;
   explicit KitVoice(std::string name) noexcept;
   std::string name;
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   struct Components : public std::vector<KitComponent>
   {
   };
   Components components;
   int noteOffset{0};
   float amp{1.0f};
   bool operator==(const KitVoice& rhs) const;
};

}   // namespace base::instruments::loader

#include "KitInstrumentVoice.inl"
#include "KitInstrumentVoiceMeta.h"

#endif
