#ifndef DEVICE_PRESETS_INL
#define DEVICE_PRESETS_INL

namespace base::musicDevice::sound::preset
{

template<class Cb>
void DevicePresets::forEachPreset(Cb cb) const
{
    for(int engineIdx = -1; engineIdx < m_presets.size() - 1; ++engineIdx)
    {
        for(const auto& preset : m_presets[engineIdx + 1])
        {
            cb(engineIdx, preset.first, preset.second);
        }
    }
}

inline int DevicePresets::engine2VectorIdx(int engineIdx) noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == engineIdx)
   {
      return 0;
   }
   return engineIdx + 1;
}

} // namespace base::musicDevice::sound::preset

#endif // DEVICE_PRESETS_INL