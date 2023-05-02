#ifndef INSTRUMENTS_COMPOSITE_SOUND_H
#define INSTRUMENTS_COMPOSITE_SOUND_H

#include "Identifiable.h"
#include "InstrumentVoice.h"
#include <string>

namespace base::instruments
{

struct CompositeSound //: public util::Identifiable
{
   CompositeSound() noexcept = default;
   inline CompositeSound(std::string name) noexcept;
   std::string name;
   //   static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
   //   using Voices = std::array<Voice, NUM_MAX_VOICES_PER_KIT_VOICE>;
   using Voices = std::vector<Voice>;
   Voices voices;
   inline bool operator==(const CompositeSound& rhs) const;
};

}   // namespace base::instruments

#include "CompositeSound.inl"
#include "CompositeSoundMeta.h"

#endif
