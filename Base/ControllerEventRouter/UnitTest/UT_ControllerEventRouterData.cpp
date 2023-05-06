#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
#include "JsonCast.h"
#include <unordered_map>

#include "ControllerEventDestination.h"
#include "ControllerEvents.h"
//#include "ControllerEventRouterData.h"

namespace base
{
struct Person
{
   std::string name;
   int age;
   int weight;
};

struct PersonHasher
{
   std::size_t operator()(const Person& p) const
   {
      std::size_t h1 = std::hash<std::string>()(p.name);
      std::size_t h2 = std::hash<int>()(p.age);
      std::size_t h3 = std::hash<int>()(p.weight);
      return h1 ^ (h2 << 1) ^ (h3 << 2);
   }
};

void to_json(nlohmann::json& j, const Person& p)
{
   j = nlohmann::json{{"name", p.name}, {"age", p.age}, {"weight", p.weight}};
}

void from_json(const nlohmann::json& j, Person& p)
{
   j.at("name").get_to(p.name);
   j.at("age").get_to(p.age);
   j.at("weight").get_to(p.weight);
}

}   // namespace base

/*
using MapDataType = std::unordered_map<std::string,
base::eventRouter::EventDestination>;
*/

namespace myns
{
// using MapDataType =
//    std::unordered_map<base::musicDevice::controller::EventIdExt,
//                       base::eventRouter::EventDestination>;

using MapDataType =
    std::unordered_map<base::musicDevice::controller::EventIdExt, base::eventRouter::EventDestination>;

void to_json(nlohmann::json& j, const MapDataType& obj) { assert(false); }

void from_json(const nlohmann::json& j, MapDataType& obj) { assert(false); }

}   // namespace myns

TEST(TestControllerEventRouterData, testSerialization)
{
   /*
  base::eventRouter::Data data;
  std::string jsonStr = meta::serialize(data).dump();
  spdlog::info("jsonStr: {}", jsonStr);
  nlohmann::json j = nlohmann::json::parse(jsonStr);
  auto retData     = j.get<base::eventRouter::Data>();
  */
   // std::unordered_map<base::musicDevice::controller::EventIdExt,
   // base::eventRouter::EventDestination> data;
   myns::MapDataType data;
   nlohmann::json jsonObj(data);
   // std::string jsonStr = meta::serialize(data).dump();
   /*
   spdlog::info("jsonStr: {}", jsonStr);
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   */
   //   auto retData     =
   //   j.get<std::unordered_map<base::musicDevice::controller::EventIdExt,
   //   base::eventRouter::EventDestination>>();

   // decltype(data) retData;
   // from_json(j, retData);

   // auto retData = j.get<decltype(data)>();
}