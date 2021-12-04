#ifndef DEVICE_PRESETS_SETTINGS_H
#define DEVICE_PRESETS_SETTINGS_H

#include <string>
#include <vector>
#include <optional>

#include "LFO.h"
#include "DevicePresetsTypes.h"

namespace base::musicDevice::sound::preset::settings
{

struct LFOData
{
   void from(
       const ::base::musicDevice::sound::preset::LFOData& lfoData) noexcept;
   ::base::musicDevice::sound::preset::LFOData to() const noexcept;
   std::optional<float> amplitude;
   std::optional<float> frequency;
   std::optional<lfo::Waveform> waveform;
   std::optional<int> multiplierExp;
};

struct ParameterData
{
   void from(const ::base::musicDevice::sound::preset::ParameterData&
                 parameterData) noexcept;
   ::base::musicDevice::sound::preset::ParameterData to() const noexcept;
   float commanded{0.0};
   std::optional<LFOData> lfoData;
};

struct Preset
{
   void from(const ::base::musicDevice::sound::preset::Preset& preset) noexcept;
   ::base::musicDevice::sound::preset::Preset to() const noexcept;
   Category category;
   Genre genre;
   std::vector<ParameterData> parameters;
};

}   // namespace base::musicDevice::sound::preset::settings

#endif