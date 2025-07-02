#ifndef INSTRUMENTS_PERSISTER_LOADER_H
#define INSTRUMENTS_PERSISTER_LOADER_H

#include <memory>

#include "IDataPersister.h"
#include "InstrumentsData.h"
// clang-format off
namespace base::musicDevice::factory { class MusicDevices; }
// clang-format on

namespace base::instruments::loader
{
class Persister
{
public:
   Persister(std::unique_ptr<util::IDataPersister> m_dataPersister,
             base::musicDevice::factory::MusicDevices& rMusicDevices);
   void save(const Data& data);
   Data load();

private:
   std::unique_ptr<util::IDataPersister> m_dataPersister;
   base::musicDevice::factory::MusicDevices& m_rMusicDevices;
};

}   // namespace base::instruments::loader

#endif
