#ifndef INSTRUMENTS_MD_REF_SETTER_RT_H
#define INSTRUMENTS_MD_REF_SETTER_RT_H

#include "InstrumentsData.h"

namespace base::instruments::loader
{
class InstrumentsMDRefSetter
{
public:
   InstrumentsMDRefSetter(Data& rData);
   void fillReferencesKitInstruments(musicDevice::MusicDevice* pMusicDevice);
   void fillReferencesMelodicInstruments(
       musicDevice::MusicDevice* pMusicDevice);
   void removeKitInstruments(musicDevice::MusicDevice* pMusicDevice);
   void removeMelodicInstruments(musicDevice::MusicDevice* pMusicDevice);

private:
   Data& m_rData;
};

}   // namespace base::instruments::loader

#endif
