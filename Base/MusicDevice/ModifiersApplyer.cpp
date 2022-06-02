#include "ModifiersApplyer.h"

using namespace base::musicDevice;

ModifiersApplyer::ModifiersApplyer(
    sound::ModifiersList &rModifiersList,
    MusicDeviceContainer &rMusicDeviceContainer) noexcept :
    m_rModifiersList(rModifiersList),
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void ModifiersApplyer::operator()() noexcept
{
   for (auto &modifier : m_rModifiersList)
   {
      if (modifier.intensity || modifier.justGotZeroIntensity)
      {
         auto mdIter = m_rMusicDeviceContainer.find(modifier.destination.uuid);
         if (mdIter != m_rMusicDeviceContainer.end() &&
             mdIter->second->soundHandler)
         {
            mdIter->second->soundHandler->applyModifier(
                modifier.destination.voiceIdx,
                modifier.destination.parameterIdx,
                modifier.destination.parameterPart, modifier.goalValue,
                modifier.intensity);
         }
      }
   }
}
