#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <Meta.h>
#include <JsonCast.h>
#include <spdlog/spdlog.h>
#include "KitInstrument.h"
#include "KitInstrumentModifier.h"

using namespace base::instruments::loader;

TEST(UT_KitSerialization, component_to_json)
{
   KitComponent component;
   nlohmann::json j;
   to_json(j, component);
   spdlog::info("{}", j.dump(3));
}

TEST(UT_KitSerialization, voice_to_json)
{
   KitVoice voice;
   nlohmann::json j;
   to_json(j, voice);
   spdlog::info("{}", j.dump(3));
}

TEST(UT_KitSerialization, instrument_to_json)
{
   KitInstrument instrument;
   KitInstrumentModifier(instrument).renameKitInstrument("test");
   nlohmann::json j;
   to_json(j, instrument);
   spdlog::info("{}", j.dump(3));
}
