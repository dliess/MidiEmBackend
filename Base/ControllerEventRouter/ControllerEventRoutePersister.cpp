#include "ControllerEventRoutePersister.h"

base::eventRouter::Persister::Persister(
    std::unique_ptr<util::IDataPersister> dataPersister) :
    m_dataPersister(std::move(dataPersister))
{
}

void base::eventRouter::Persister::save(const Data& data)
{
   //m_dataPersister->save(meta::serialize(data).dump().c_str());
}

base::eventRouter::Data base::eventRouter::Persister::load()
{
   const auto strData = m_dataPersister->load();
   nlohmann::json j   = nlohmann::json::parse(strData);
   return Data{};
   //TODO return j.get<Data>();
}
