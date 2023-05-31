#include <gtest/gtest.h>

#include "KitInstrument.h"
#include "MelodicInstrument.h"

using namespace base::instruments;

TEST(InstrumentSerializationTest, melodicInstrument)
{
   MelodicInstrument melodicInstrument(
       "A test music device", std::make_shared<MelodicInstrument::RtData>());
   melodicInstrument.voices().emplace_back("sound1");
   melodicInstrument.voices().at(0).components[0] = Component(
       nullptr, std::make_shared<Component::ParameterCache>(12),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = nlohmann::json(melodicInstrument).dump();
   spdlog::info(jsonStr);
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   auto data        = j.get<MelodicInstrument>();
}

TEST(InstrumentSerializationTest, component)
{
   Component component(
       nullptr, std::make_shared<Component::ParameterCache>(12),
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   std::string jsonStr = nlohmann::json(component).dump();
   spdlog::info(jsonStr);
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   auto data        = j.get<Component>();
}

TEST(InstrumentSerializationTest, KitVoiceComponents)
{
   KitVoice::Components components;
   nlohmann::json j(components);
   KitVoice::Components componentsRet;
   j.get_to(componentsRet);
}


TEST(InstrumentSerializationTest, MelodicVoiceComponents)
{
   MelodicVoice::Components components;
   components[0] = Component(
       nullptr, nullptr,
       base::musicDevice::MusicDeviceId("Novation/Circuit", "1235:0079"), 4, 3);
   components[2] = Component(
       nullptr, nullptr,
       base::musicDevice::MusicDeviceId("Elektron/AnalogFourMk1", "1233:0049"), 1, 1);
   nlohmann::json j(components);
   std::cout << j.dump()<< "\n";
   MelodicVoice::Components componentsRet;
   j.get_to(componentsRet);
   ASSERT_EQ(true, componentsRet[0].has_value());
   ASSERT_EQ(false, componentsRet[1].has_value());
   ASSERT_EQ(true, componentsRet[2].has_value());
   ASSERT_EQ(false, componentsRet[3].has_value());
}
