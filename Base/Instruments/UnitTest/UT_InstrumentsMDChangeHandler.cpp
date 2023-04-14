#include <gtest/gtest.h>

#include "InstrumentsMDChangeHandler.h"
#include "MusicDeviceFactoryDataHolder.h"
#include "Instruments.h"

using namespace base::musicDevice;
using namespace base::instruments;

TEST(InstrumentsMDChangeHandlerTest, test)
{
   factory::DataHolder factoryDataHolder("~");
   Instruments instruments(factoryDataHolder);
   InstrumentsMDChangeHandler instrumentsMDChangeHandler(instruments);
}