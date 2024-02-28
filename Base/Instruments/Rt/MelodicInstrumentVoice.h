#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_RT_H
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_RT_H

#include <string>

#include "Identifiable.h"
#include "MusicDeviceId.h"
#include "SoundHandler.h"
#include <array>

namespace base::instruments::rt
{
struct MelodicVoice   //: public util::Identifiable
{
   struct Component
   {
      musicDevice::MusicDeviceId soundDeviceId;
      int sdVoiceIdx{0};
      musicDevice::sound::SoundHandler* soundHandler{nullptr};
   };
   static constexpr std::size_t NUM_MAX_COMPONENTS = 4;
   using Components = std::array<std::optional<Component>, NUM_MAX_COMPONENTS>;

   Components components;
};

}   // namespace base::instruments::rt

#endif
