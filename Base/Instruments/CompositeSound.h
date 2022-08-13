#ifndef INSTRUMENTS_COMPOSITE_SOUND_H
#define INSTRUMENTS_COMPOSITE_SOUND_H

#include "Identifiable.h"
#include "VoiceDescr.h"
#include <string>

namespace base::instruments
{

struct CompositeSound : public util::Identifiable
{
   inline CompositeSound() noexcept = default;
   inline CompositeSound(std::string name) noexcept;
   std::string name;
   //   static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
   //   using Voices = std::array<VoiceDescr, NUM_MAX_VOICES_PER_KIT_VOICE>;
   using Voices = std::vector<VoiceDescr>;
   Voices voices;
};

}   // namespace base::instruments

#include "CompositeSound.inl"

#endif
