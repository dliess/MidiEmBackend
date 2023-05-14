#include "ControllerEventRoutePersister.h"

base::eventRouter::Persister::Persister(
    std::unique_ptr<util::IDataPersister> dataPersister) :
    m_dataPersister(std::move(dataPersister))
{
}

void base::eventRouter::Persister::save(const LoaderData& data)
{
   m_dataPersister->save(nlohmann::json(data).dump().c_str());
}

base::eventRouter::LoaderData base::eventRouter::Persister::load()
{
   const auto strData = m_dataPersister->load();
   nlohmann::json j   = nlohmann::json::parse(strData);
   return j.get<LoaderData>();
}
