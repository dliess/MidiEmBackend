#ifndef INSTRUMENTS_KIT_INSTRUMENTS_VOICE_H
#define INSTRUMENTS_KIT_INSTRUMENTS_VOICE_H

#include <string>

#include "Identifiable.h"
#include "InstrumentComponent.h"
#include <vector>

namespace base::instruments
{
struct KitVoice   //: public util::Identifiable
{
   KitVoice() noexcept = default;
   explicit KitVoice(std::string name) noexcept;
   std::string name;
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   struct Components : public std::vector<Component>
   {
      [[nodiscard]] bool containsComponent(size_t componentIdx) const noexcept
      {
         return componentIdx < size();
      }
   };
   Components components;
   float amp{1.0f};
   bool operator==(const KitVoice& rhs) const;
};

}   // namespace base::instruments

#include "KitInstrumentVoice.inl"
#include "KitInstrumentVoiceMeta.h"

#endif
