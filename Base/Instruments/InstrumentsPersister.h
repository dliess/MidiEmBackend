#ifndef INSTRUMENTS_PERSISTER_H
#define INSTRUMENTS_PERSISTER_H

#include <memory>

#include "IDataPersister.h"

namespace base::instruments
{
class Persister
{
public:
   Persister(std::unique_ptr<util::IDataPersister> m_dataPersister);
   void save(const Data& data);
   Data load();

private:
   std::unique_ptr<util::IDataPersister> m_dataPersister;
};

}   // namespace base::instruments

#endif