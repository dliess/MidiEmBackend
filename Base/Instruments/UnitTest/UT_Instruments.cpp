#include <gtest/gtest.h>

#include "MusicDeviceContainer.h"
#include "Instruments.h"

using namespace base::musicDevice;
using namespace base::instruments;

TEST(InstrumentsTest, test)
{
   MusicDeviceContainer musicDeviceContainer;
   Instruments instruments(musicDeviceContainer);
}