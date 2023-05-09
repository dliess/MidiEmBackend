#include <gtest/gtest.h>

#include "ControllerEvents.h"
#include "Meta.h"

TEST(ControllerEvents, EventIdExt_SerializationTest)
{
   base::musicDevice::controller::EventIdExt eventIdExt;
   const auto j = nlohmann::json(eventIdExt);
   const auto jString = j.dump();
   nlohmann::json jRet = nlohmann::json::parse(jString);
   auto retData = meta::deserialize<base::musicDevice::controller::EventIdExt>(jRet);
   ASSERT_EQ(eventIdExt, retData);
}