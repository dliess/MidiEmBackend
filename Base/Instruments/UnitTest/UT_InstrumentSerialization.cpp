#include <gtest/gtest.h>

#include "MelodicInstrument.h"

using namespace base::instruments;


TEST(InstrumentSerializationTest, melodicInstrument)
{
   MelodicInstrument melodicInstrument(
       "A test music device", std::make_shared<MelodicInstrument::RtData>());
   melodicInstrument.voices().emplace_back("sound1");
   melodicInstrument.voices().at(0).voices.emplace_back(
       nullptr, std::make_shared<Voice::ParameterCache>(),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = meta::serialize(melodicInstrument).dump();
   spdlog::info(jsonStr);
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   auto data        = j.get<MelodicInstrument>();
}

TEST(InstrumentSerializationTest, voice)
{
   Voice voice(
       nullptr, std::make_shared<Voice::ParameterCache>(),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = meta::serialize(voice).dump();
   spdlog::info(jsonStr);
   nlohmann::json j    = nlohmann::json::parse(jsonStr);
   auto data           = j.get<Voice>();
}