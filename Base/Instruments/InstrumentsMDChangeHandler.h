#ifndef INSTRUMENTS_MD_CHANGE_HANDLER_H
#define INSTRUMENTS_MD_CHANGE_HANDLER_H

#include "InstrumentsData.h"
#include "DoubleBuffer.h"

// clang-format off
namespace base::musicDevice { class MusicDevice; }
// clang-format on
namespace base::instruments
{

class InstrumentsMDChangeHandler
{
public:
   InstrumentsMDChangeHandler(
       util::DoubleBuffer<Data>& rDoubleBufferedData) noexcept;
   void add(musicDevice::MusicDevice* pMusicDevice);
   void remove(musicDevice::MusicDevice* pMusicDevice);

private:
   util::DoubleBuffer<Data>& m_rDoubleBufferedData;

   void fillReferences(
       musicDevice::MusicDevice* pMusicDevice) noexcept;

   void addDefaultInstrumentsFor(
       musicDevice::MusicDevice* pMusicDevice) noexcept;
   void addDefaultInstrumentsForDrumKit(musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForInstrumentPerVoice(musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForOnePolyphonicInstrument(musicDevice::MusicDevice* pMusicDevice);
};

}   // namespace base::instruments
#endif