#include "InstrumentsMDRefSetter.h"

using namespace base;
using namespace base::instruments::loader;

InstrumentsMDRefSetter::InstrumentsMDRefSetter(Data& rData) : m_rData(rData) {}


namespace detail
{
template <class InstrumentContainer>
void removeReference(InstrumentContainer&& instrumentContainer,
                     musicDevice::MusicDevice* pMusicDevice)
{
   auto it = instrumentContainer.begin();
   while (it != instrumentContainer.end())
   {
      bool isDeviceContained{false};
      it->forEachComponent([&isDeviceContained,
                            &pMusicDevice](Component& component) {
         if (component.pSoundDevice() == &pMusicDevice->soundHandler.value())
         {
            isDeviceContained = true;
            component.setSoundDevicePtr(nullptr);
         }
      });
      if (isDeviceContained && it->isDefaultCreated() && it->refCount() == 0)
      {
         it = instrumentContainer.erase(it);
      }
      else
      {
         ++it;
      }
   }
}
}   // namespace detail

void InstrumentsMDRefSetter::removeKitInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   detail::removeReference(m_rData.kitInstruments, pMusicDevice);
}

void InstrumentsMDRefSetter::removeMelodicInstruments(
    musicDevice::MusicDevice* pMusicDevice)
{
   detail::removeReference(m_rData.melodicInstruments, pMusicDevice);
}
