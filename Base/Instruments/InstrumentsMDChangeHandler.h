#ifndef INSTRUMENTS_MD_CHANGE_HANDLER_H
#define INSTRUMENTS_MD_CHANGE_HANDLER_H

#include "InstrumentsData.h"

// clang-format off
namespace base::musicDevice { class MusicDevice; }
// clang-format on
namespace base::instruments
{
class Instruments;
class InstrumentsMDChangeHandler
{
public:
   InstrumentsMDChangeHandler(Instruments& m_rInstruments) noexcept;
   void add(musicDevice::MusicDevice* pMusicDevice);
   void remove(musicDevice::MusicDevice* pMusicDevice);

private:
   Instruments& m_rInstruments;

   void addDefaultInstrumentsFor(
       musicDevice::MusicDevice* pMusicDevice) noexcept;
   void addDefaultInstrumentsForDrumKit(musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForInstrumentPerVoice(
       musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForOnePolyphonicInstrument(
       musicDevice::MusicDevice* pMusicDevice);
};

}   // namespace base::instruments
#endif