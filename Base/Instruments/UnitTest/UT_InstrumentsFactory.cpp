#include <gtest/gtest.h>

#include "InstrumentsMDChangeHandler.h"
#include "Instruments.h"
#include "MusicDeviceHolder.h"

using namespace base::instruments;

TEST(InstrumentsFactoryTest, test)
{
   base::musicDevice::Holder musicDeviceHolder;
   Instruments instruments(musicDeviceHolder.musicDevices);
   InstrumentsMDChangeHandler instrumentsFactory(instruments, musicDeviceHolder);
}