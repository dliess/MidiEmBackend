#include <gtest/gtest.h>

#include "ControllerEventRouterData.h"

TEST(TestControllerEventRouterData, testSerialization)
{
   base::eventRouter::Data data;
   std::string jsonStr = meta::serialize(data).dump();
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   auto retData     = j.get<base::eventRouter::Data>();
}