#ifndef CONTROLLER_EVENT_ROUTE_PERSISTER_H
#define CONTROLLER_EVENT_ROUTE_PERSISTER_H

#include <memory>

#include "IDataPersister.h"
#include "ControllerEventRouter.h"

namespace base::eventRouter
{
class Persister
{
public:
   explicit Persister(std::unique_ptr<util::IDataPersister> dataPersister);
   void save(const EventRouter::Data& data);
   EventRouter::Data load();

private:
   std::unique_ptr<util::IDataPersister> m_dataPersister;
};

}   // namespace base::eventRouter

#endif