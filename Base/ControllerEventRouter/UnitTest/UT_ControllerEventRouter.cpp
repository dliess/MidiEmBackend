#include <gtest/gtest.h>

#include "Mock/Mock_Instruments.h"
#include "Mock/Mock_MusicDeviceContainer.h"
#include "ControllerEventRouter.h"

using namespace base::musicDevice;

TEST(TestControllerEventRouter, test)
{
   base::instruments::mock::MockInstruments instrumentsMOCK;
   base::musicDevice::mock::MockMusicDeviceContainer musicDeviceContainerMOCK;
   base::musicDevice::controller::EventRouter eventRouter(instrumentsMOCK, musicDeviceContainerMOCK);
}