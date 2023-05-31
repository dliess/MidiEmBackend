#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_H
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_H

#include <string>

#include "Identifiable.h"
#include "InstrumentComponent.h"
#include <array>

namespace base::instruments
{
struct MelodicVoice   //: public util::Identifiable
{
   MelodicVoice() noexcept = default;
   explicit MelodicVoice(std::string name) noexcept;
   std::string name;
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   struct Components : public std::array<std::optional<Component>, NUM_MAX_COMPONENTS_PER_VOICE>
   {
      [[nodiscard]] bool containsComponent(size_t componentIdx) const noexcept
      {
         return at(componentIdx).has_value();
      }
   };
   Components components;
   bool operator==(const MelodicVoice& rhs) const;
};

}   // namespace base::instruments

#include "MelodicInstrumentVoice.inl"
#include "MelodicInstrumentVoiceMeta.h"

#endif
