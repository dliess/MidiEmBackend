#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "JsonCast.h"
#include <unordered_map>

#include "ControllerEventRouterData.h"

TEST(TestControllerEventRouterData, testSerialization)
{
  base::eventRouter::Data data;
  nlohmann::json jsonObj(data);
  const auto jsonStr = jsonObj.dump();
  nlohmann::json jRet = nlohmann::json::parse(jsonStr);
  base::eventRouter::Data retData;
  jRet.get_to(retData);
}