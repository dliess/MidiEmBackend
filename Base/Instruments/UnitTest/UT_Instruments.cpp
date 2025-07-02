#include <gtest/gtest.h>

#include "Instruments.h"
#include "Refs/InstrumentsRef.h"
#include "MusicDeviceFactoryDataHolder.h"

using namespace base::musicDevice;
using namespace base::instruments;

TEST(InstrumentsTest, test)
{
   factory::MusicDevices factoryDataHolder("~");
   Instruments instruments(factoryDataHolder);
   InstrumentsRef instrumentsRef(instruments);
}
