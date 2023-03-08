#include "InstrumentsPersister.h"

#include "InstrumentVoiceFactory.h"
#include "MusicDeviceFactoryDataHolder.h"

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

Persister::Persister(
    std::unique_ptr<util::IDataPersister> dataPersister,
    base::musicDevice::factory::DataHolder rFactoryDataHolder) :
    m_dataPersister(std::move(dataPersister)),
    m_rFactoryDataHolder(rFactoryDataHolder)
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
      instr.forEachVoice([](auto& voice) {
         voice.m_pParameterCache = createParameterCache(
             m_rFactoryDataHolder.getDescription(voice.m_soundDeviceId),
             voice.m_voiceIndex);
      });
   }
   for (auto& instr : data.melodicInstruments)
   {
      instr.forEachVoice([](auto& voice) {
         voice.m_pParameterCache = createParameterCache(
             m_rFactoryDataHolder.getDescription(voice.m_soundDeviceId),
             voice.m_voiceIndex);
      });
   }
   return data;
}
