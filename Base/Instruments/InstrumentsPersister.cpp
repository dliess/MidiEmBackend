#include "InstrumentsPersister.h"

using namespace base::instruments;

namespace detail
{
instruments::Data filterOutDefaultInstruments(const instruments::Data& rData)
{
   instruments::Data data = rData;
   for (auto it = data.kitInstruments.begin(); it != data.kitInstruments.end();
        ++it)
   {
      if (it->isDefaultCreated() && it->refCount() == 0)
      {
         it = data.kitInstruments.erase(it);
      }
   }
   for (auto it = data.melodicInstruments.begin();
        it != data.melodicInstruments.end(); ++it)
   {
      if (it->isDefaultCreated() && it->refCount() == 0)
      {
         it = data.melodicInstruments.erase(it);
      }
   }
   return data;
}

}   // namespace detail

Persister::Persister(std::unique_ptr<util::IDataPersister> dataPersister) :
    m_dataPersister(std::move(dataPersister))
{
}
void Persister::save(const Data& data)
{
   m_dataPersister->save(
       meta::serialize(detail::filterOutDefaultInstruments(data))
           .dump()
           .c_str());
}

Data Persister::load()
{
   const auto strData = m_dataPersister->load();
   nlohmann::json j   = nlohmann::json::parse(strData);
   auto data          = j["section"].get<Data>();
   for (auto& instr : data.kitInstruments)
   {
      instr.forEachVoice([](auto& voice) { voice.setParameterCache(); });
   }
   for (auto& instr : data.melodicInstruments)
   {
      instr.forEachVoice([](auto& voice) { voice.setParameterCache(); });
   }
}
