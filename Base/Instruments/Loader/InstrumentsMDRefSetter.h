#ifndef INSTRUMENTS_MD_REF_SETTER_LOADER_H
#define INSTRUMENTS_MD_REF_SETTER_LOADER_H

#include "InstrumentsData.h"

namespace base::instruments::loader
{
class InstrumentsMDRefSetter
{
public:
   InstrumentsMDRefSetter(Data& rData);
   void removeKitInstruments(musicDevice::MusicDevice* pMusicDevice);
   void removeMelodicInstruments(musicDevice::MusicDevice* pMusicDevice);

private:
   Data& m_rData;
};

}   // namespace base::instruments::loader

#endif
