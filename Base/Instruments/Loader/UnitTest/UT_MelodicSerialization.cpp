#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <Meta.h>
#include <JsonCast.h>
#include <spdlog/spdlog.h>
#include "MelodicInstrument.h"
#include "MelodicInstrumentModifier.h"

using namespace base::instruments::loader;

TEST(UT_MelodicSerialization, component_to_json)
{
   MelodicInstrument::Voice::Component component{{"test-deviceName", "test-portName"}, 3};
   nlohmann::json j;
   to_json(j, component);
   spdlog::info("{}", j.dump(3));
}

TEST(UT_MelodicSerialization, component_from_json)
{
   MelodicInstrument::Voice::Component data;
   const std::string jsonStr = R"({"sdVoiceIdx": 0, "soundDeviceId": { "deviceName": "hello", "portName": "bello"  } })";
   nlohmann::json j = nlohmann::json::parse(jsonStr);
   spdlog::info("{}", j.dump(3));
   ASSERT_NO_THROW(from_json(j, data));
}

TEST(UT_MelodicSerialization, voice_to_json)
{
   MelodicInstrument::Voice voice;
   nlohmann::json j;
   to_json(j, voice);
   spdlog::info("{}", j.dump(3));
}

TEST(UT_MelodicSerialization, instrument_to_json)
{
   MelodicInstrument melodicInstrument;
   MelodicInstrumentModifier(melodicInstrument).renameMelodicInstrument("test");
   nlohmann::json j;
   to_json(j, melodicInstrument);
   spdlog::info("{}", j.dump(3));
}

