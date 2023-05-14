#ifndef CONTROLLER_EVENT_ROUTER_DATA_CONVERTER_H
#define CONTROLLER_EVENT_ROUTER_DATA_CONVERTER_H

#include "ControllerEventRouterData.h"
#include "MusicDeviceFactoryDataHolder.h"

namespace base::eventRouter
{

class DataConverter
{
public:
explicit DataConverter(const base::musicDevice::factory::DataHolder& rMDFDataHolder) :
    m_rMDFDataHolder(rMDFDataHolder)
    {}

Data convertFromLoaded(const LoaderData& loaderData)
{
    Data ret;
    for(const auto& [eventId, eventDest] : loaderData)
    {
        auto uuid = m_rMDFDataHolder.getUUIDByMdId(eventId.uuid);
        if(uuid)
        {
            ret.try_emplace({*uuid, eventId.eventId}, eventDest);
        }
    }
    return ret;
}

LoaderData convertToLoaded(const Data& data)
{
    LoaderData ret;
    for(const auto& [eventId, eventDest] : data)
    {
        auto mdId = m_rMDFDataHolder.getMdIdByUUID(eventId.uuid);
        if(mdId)
        {
            ret.try_emplace({*mdId, eventId.eventId}, eventDest);
        }
    }
    return ret;
}
private:
    const base::musicDevice::factory::DataHolder& m_rMDFDataHolder;
};

}   // namespace base::eventRouter


#endif