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
   preset.parameters.emplace_back(0.0, LFOData{0.0, 1.0, lfo::Waveform::Sine, 0});
   auto j = nlohmann::json(preset);

}