#include <gtest/gtest.h>
#include "DevicePresets.h"

inline
bool operator==(const base::musicDevice::sound::preset::LFOData& lhs, 
                const base::musicDevice::sound::preset::LFOData& rhs)
{
   return lhs.amplitude == rhs.amplitude &&
          lhs.frequency == rhs.frequency &&
          lhs.waveform == rhs.waveform &&
          lhs.multiplierExp == rhs.multiplierExp;
}

inline
bool operator==(const base::musicDevice::sound::preset::ParameterData& lhs, 
                const base::musicDevice::sound::preset::ParameterData& rhs)
{
   return lhs.commanded == rhs.commanded &&
          lhs.lfoData == rhs.lfoData;
}

using namespace base::musicDevice::sound;
using namespace base::musicDevice::sound::preset;

TEST(DevicePresetSerializationTest, serialize)
{
   Preset preset;
   preset.slotOnDeviceIndex = 1;
   preset.category = Category::None;
   preset.genre = Genre::None;
   preset.parameters.push_back(ParameterData{0.16265715317, LFOData{0.0, 1.0, lfo::Waveform::Sine, 0}});
   preset.parameters.push_back(ParameterData{0.16235715317, LFOData{0.316265715317, 0.9, lfo::Waveform::Sine, 0}});
   preset.parameters.push_back(ParameterData{0.99999999999, LFOData{0.4, 0.816265715317, lfo::Waveform::Sine, 0}});
   preset.parameters.push_back(ParameterData{0.0, LFOData{0.5, 0.0, lfo::Waveform::Sine, 0}});
   auto j = nlohmann::json(preset);
   ASSERT_EQ(j.dump(), "{\"category\":\"None\",\"genre\":\"None\",\"parameters\":[{\"commanded\":0.163},{\"commanded\":0.162,\"lfo\":{\"amplitude\":0.316,\"frequency\":0.9}},{\"commanded\":1.0,\"lfo\":{\"amplitude\":0.4,\"frequency\":0.816}},{\"commanded\":0.0}]}");
}

TEST(DevicePresetSerializationTest, deserialize)
{
   auto j = nlohmann::json::parse("{\"category\":\"None\",\"genre\":\"None\",\"parameters\":[{\"commanded\":0.163},{\"commanded\":0.162,\"lfo\":{\"amplitude\":0.316,\"frequency\":0.9}},{\"commanded\":1.0,\"lfo\":{\"amplitude\":0.4,\"frequency\":0.816}},{\"commanded\":0.0}]}");
   auto preset = j.get<Preset>();
   ASSERT_EQ(preset.slotOnDeviceIndex, std::nullopt);
   ASSERT_EQ(preset.category, Category::None);
   ASSERT_EQ(preset.genre, Genre::None);
   ASSERT_EQ(preset.parameters.size(), 4);
   EXPECT_NEAR(preset.parameters[0].commanded, 0.163, 0.0001);
   EXPECT_NEAR(preset.parameters[0].lfoData.amplitude, 0.0, 0.0001);
   EXPECT_NEAR(preset.parameters[0].lfoData.frequency, 0.0, 0.0001);
   ASSERT_EQ(preset.parameters[0].lfoData.waveform, lfo::Waveform::Sine);
   ASSERT_EQ(preset.parameters[0].lfoData.multiplierExp, 0);
}