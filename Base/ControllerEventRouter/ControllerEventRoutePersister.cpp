#include "ControllerEventRoutePersister.h"

using namespace base::eventRouter;

Persister::Persister(std::unique_ptr<util::IDataPersister> dataPersister) :
   m_dataPersister(std::move(dataPersister))
{

}

void Persister::save(const EventRouter::Data& data)
{

}

EventRouter::Data Persister::load()
{

}
