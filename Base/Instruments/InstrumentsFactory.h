#ifndef INSTRUMENTS_FACTORY_H
#define INSTRUMENTS_FACTORY_H

#include <memory>

namespace base
{
namespace musicDevice
{
struct Holder;
struct MusicDevice;
} // namespace musicDevice
namespace instruments
{
struct Instruments;

class InstrumentsFactory
{
public:
   InstrumentsFactory(Instruments& rInstruments,
                      musicDevice::Holder& rMusicDeviceHolder) noexcept;

private:
   Instruments& m_rInstruments;

private:
   void add(std::shared_ptr<musicDevice::MusicDevice> pMusicDevice);
   void remove(std::shared_ptr<musicDevice::MusicDevice> pMusicDevice);
   void fillReferencesInOtherInstruments(
      const std::shared_ptr<musicDevice::MusicDevice>& pMusicDevice) noexcept;
   void addDefaultInstrumentsFor(
      const std::shared_ptr<musicDevice::MusicDevice>& pMusicDevice) noexcept;
};

} // namespace instruments
} // namespace base
#endif