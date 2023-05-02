#ifndef CONTROLLER_EVENT_ROUTE_PERSISTER_H
#define CONTROLLER_EVENT_ROUTE_PERSISTER_H

#include <memory>

#include "IDataPersister.h"
#include "ControllerEventRouterData.h"

namespace base::eventRouter
{
class Persister
{
public:
   explicit Persister(std::unique_ptr<util::IDataPersister> dataPersister);
   void save(const Data& data);
   Data load();

private:
   std::unique_ptr<util::IDataPersister> m_dataPersister;
};

}   // namespace base::eventRouter

#endif