#include <gtest/gtest.h>

#include "Instruments.h"
#include "Refs/InstrumentsRef.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "MusicDeviceContainer.h"

using namespace base;

TEST(InstrumentsTest, test)
{
   musicDevice::factory::MusicDevices factoryMusicDevices("Config dir path");
   musicDevice::MusicDeviceContainer rtMusicDeviceContainer; 
   instruments::Instruments instruments(factoryMusicDevices, rtMusicDeviceContainer);
   instruments::InstrumentsRef instrumentsRef(instruments);
}
