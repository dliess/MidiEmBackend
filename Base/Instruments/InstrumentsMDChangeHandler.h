#ifndef INSTRUMENTS_MD_CHANGE_HANDLER_H
#define INSTRUMENTS_MD_CHANGE_HANDLER_H

// clang-format off
namespace base::musicDevice { class MusicDevice; }
// clang-format on
namespace base::instruments
{
struct Data;

class InstrumentsMDChangeHandler
{
public:
   InstrumentsMDChangeHandler(
       Data& rData) noexcept;
   void add(musicDevice::MusicDevice* pMusicDevice);
   void remove(musicDevice::MusicDevice* pMusicDevice);

private:
   Data& m_rData;

   void removeKitInstruments(musicDevice::MusicDevice* pMusicDevice);
   void removeMelodicInstruments(musicDevice::MusicDevice* pMusicDevice);

   void fillReferences(
       musicDevice::MusicDevice* pMusicDevice) noexcept;
   void fillReferencesKitInstruments(
       musicDevice::MusicDevice* pMusicDevice) noexcept;
   void fillReferencesMelodicInstruments(
       musicDevice::MusicDevice* pMusicDevice) noexcept;

   void addDefaultInstrumentsFor(
       musicDevice::MusicDevice* pMusicDevice) noexcept;
   void addDefaultInstrumentsForDrumKit(musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForInstrumentPerVoice(musicDevice::MusicDevice* pMusicDevice);
   void addDefaultInstrumentsForOnePolyphonicInstrument(musicDevice::MusicDevice* pMusicDevice);
};

}   // namespace base::instruments
#endif