#include <gtest/gtest.h>
#include "DevicePresets.h"

using namespace base::musicDevice::sound;
using namespace base::musicDevice::sound::preset;

TEST(DevicePresetSerializationTest, test)
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