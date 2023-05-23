#include <gtest/gtest.h>

#include "MelodicInstrument.h"

using namespace base::instruments;


TEST(InstrumentSerializationTest, melodicInstrument)
{
   MelodicInstrument melodicInstrument(
       "A test music device", std::make_shared<MelodicInstrument::RtData>());
   melodicInstrument.components().emplace_back("sound1");
   melodicInstrument.components().at(0).components.emplace_back(
       nullptr, std::make_shared<Component::ParameterCache>(),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = nlohmann::json(melodicInstrument).dump();
   spdlog::info(jsonStr);
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   auto data        = j.get<MelodicInstrument>();
}

TEST(InstrumentSerializationTest, component)
{
   Component component(
       nullptr, std::make_shared<Component::ParameterCache>(),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = nlohmann::json(component).dump();
   spdlog::info(jsonStr);
   nlohmann::json j    = nlohmann::json::parse(jsonStr);
   auto data           = j.get<Component>();
}