#ifndef BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H
#define BASE_MUSIC_DEVICE_MODIFIERS_APPLYER_H

#include "ParameterSceneContainer.h"
#include "MusicDeviceContainer.h"

namespace base::musicDevice
{
class ModifiersApplyer
{
public:
   ModifiersApplyer(sound::ParameterSceneContainer &rParameterSceneContainer,
                    MusicDeviceContainer &rMusicDeviceContainer) noexcept;
   void operator()() noexcept;

private:
   sound::ParameterSceneContainer &m_rParameterSceneContainer;
   MusicDeviceContainer &m_rMusicDeviceContainer;
};

}   // namespace base::musicDevice
#endif