#include <gtest/gtest.h>

#include "InstrumentsMDChangeHandler.h"

using namespace base::instruments;

TEST(InstrumentsMDChangeHandlerTest, test)
{
   base::instruments::Data data;
   InstrumentsMDChangeHandler instrumentsFactory(data, musicDeviceHolder);
}