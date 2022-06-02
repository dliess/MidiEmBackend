#ifndef BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H
#define BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H

#include "ModifiersList.h"
#include "MusicDeviceContainer.h"

namespace base::musicDevice
{
class ModifiersApplyer
{
public:
   ModifiersApplyer(sound::ModifiersList &rModifiersList,
                    MusicDeviceContainer &rMusicDeviceContainer) noexcept;
   void operator()() noexcept;

private:
   sound::ModifiersList &m_rModifiersList;
   MusicDeviceContainer &m_rMusicDeviceContainer;
};

}   // namespace base::musicDevice
#endif