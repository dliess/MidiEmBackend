#include "ControllerAdditionalEventCreator.h"

#include "ControllerSection.h"

using namespace base::musicDevice::controller;

AdditionalEventCreator::AdditionalEventCreator(
    const description::controller::Section& rControllerSection) :
    m_rControllerSection(rControllerSection)
{
}

void AdditionalEventCreator::eventReceived(const Event& event)
{
   emitEventHappened(event);
}