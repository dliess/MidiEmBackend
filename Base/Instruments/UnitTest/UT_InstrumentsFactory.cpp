#include <gtest/gtest.h>

#include "InstrumentsFactory.h"
#include "Instruments.h"
#include "MusicDeviceHolder.h"

using namespace base::instruments;

TEST(InstrumentsFactoryTest, test)
{
   base::musicDevice::Holder musicDeviceHolder;
   Instruments instruments(musicDeviceHolder.musicDevices);
   InstrumentsFactory instrumentsFactory(instruments, musicDeviceHolder);
}