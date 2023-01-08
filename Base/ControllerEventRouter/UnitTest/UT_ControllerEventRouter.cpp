#include <gtest/gtest.h>

#include "ControllerEventRouter.h"

using namespace base::musicDevice;

TEST(TestControllerEventRouter, test)
{
   InstrumentsMOC
   ControllerEventRouter<InstrumentsMOC, MusicDeviceContainerMOC> controllerEventRouter();
}