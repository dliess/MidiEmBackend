#ifndef INSTRUMENT_VOICE_SOUND_H
#define INSTRUMENT_VOICE_SOUND_H

#include "Identifiable.h"
#include "InstrumentComponent.h"
#include <string>

namespace base::instruments
{

struct Voice //: public util::Identifiable
{
   Voice() noexcept = default;
   explicit Voice(std::string name) noexcept;
   std::string name;
   static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
   //   using Components = std::array<Component, NUM_MAX_COMPONENTS_PER_KIT_VOICE>;
   //using Components = std::vector<Component>;
   struct Components : public std::vector<Component> 
   {
    [[nodiscard]] bool containsComponent(size_t componentIdx) const noexcept { return componentIdx < size();}
   };
   Components components;
   bool operator==(const Voice& rhs) const;
};

}   // namespace base::instruments

#include "InstrumentVoice.inl"
#include "InstrumentVoiceMeta.h"

#endif
