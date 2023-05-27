#ifndef INSTRUMENTS_PERSISTER_H
#define INSTRUMENTS_PERSISTER_H

#include <memory>

#include "IDataPersister.h"
#include "InstrumentsData.h"

// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
// clang-format on

namespace base::instruments
{
class Persister
{
public:
   Persister(std::unique_ptr<util::IDataPersister> m_dataPersister,
             base::musicDevice::factory::DataHolder& rFactoryDataHolder);
   void save(const Data& data);
   Data load();

private:
   std::unique_ptr<util::IDataPersister> m_dataPersister;
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
};

}   // namespace base::instruments

#endif