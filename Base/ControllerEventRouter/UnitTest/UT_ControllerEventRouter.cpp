#include <gtest/gtest.h>

#include "Mock_Instruments.h"
#include "Mock_MusicDeviceContainer.h"
#include "ControllerEventRouter.h"

TEST(TestControllerEventRouter, test)
{
   base::instruments::mock::MockInstruments instrumentsMOCK;
   base::musicDevice::mock::MockMusicDeviceContainer musicDeviceContainerMOCK;
   base::eventRouter::EventRouter eventRouter(instrumentsMOCK, musicDeviceContainerMOCK);
}