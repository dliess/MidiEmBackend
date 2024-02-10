#include "InstrumentsPersister.h"

// #include "InstrumentComponentParameterCacheCreator.h"
#include "MusicDeviceFactoryDataHolder.h"

using namespace base::instruments::loader;

namespace detail
{
Data filterOutDefaultInstruments(const Data& rData)
{
   Data data = rData;
   for (auto it = data.kitInstruments.begin(); it != data.kitInstruments.end();)
   {
      if (it->isDefaultCreated() && it->refCount() == 0)
      {
         it = data.kitInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
   for (auto it = data.melodicInstruments.begin();
        it != data.melodicInstruments.end();)
   {
      if (it->isDefaultCreated() && it->refCount() == 0)
      {
         it = data.melodicInstruments.erase(it);
      }
      else
      {
         ++it;
      }
   }
   return data;
}

}   // namespace detail

Persister::Persister(
    std::unique_ptr<util::IDataPersister> dataPersister,
    base::musicDevice::factory::DataHolder& rFactoryDataHolder) :
    m_dataPersister(std::move(dataPersister)),
    m_rFactoryDataHolder(rFactoryDataHolder)
{
}
void Persister::save(const Data& data)
{
   m_dataPersister->save(
       nlohmann::json(detail::filterOutDefaultInstruments(data))
           .dump()
           .c_str());
}

Data Persister::load()
{
   const auto strData = m_dataPersister->load();
   nlohmann::json j   = nlohmann::json::parse(strData);
   auto data          = j.get<Data>();
   return data;
}
