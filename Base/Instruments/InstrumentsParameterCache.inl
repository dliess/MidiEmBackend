#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_INL
#define BASE_INSTRUMENTS_PARAMETER_CACHE_INL

#include "InstrumentsParameterCache.h"

namespace base::instruments
{
inline ParameterCache::ParameterCache(size_t size) :
    data_(size), nonRtBackupData_(size), dirtyFlags_(size)
{
}

inline std::size_t ParameterCache::size() const { return data_.size(); }

inline const ParameterCache::ParameterData& ParameterCache::at(
    std::size_t pos) const
{
   return data_.at(pos);
}

inline void ParameterCache::setParameter(
    std::size_t index, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   musicDevice::sound::setParameterData(data_.at(index), parameterAttr, value);
   dirtyFlags_.set(index, parameterAttr);
}

inline void ParameterCache::updateParameterUI()
{
   if (dirtyFlags_.any())
   {
      dirtyFlags_.forEach(
          [this](size_t paramIdx,
                 musicDevice::sound::ParameterAttr parameterAttr) {
             emitDataChangedUI(paramIdx, parameterAttr,
                               musicDevice::sound::getParameterData(
                                   data_.at(paramIdx), parameterAttr));
          });
      dirtyFlags_.reset();
   }
}

inline void ParameterCache::emiAllNonNullParameters()
{
   for (int paramIdx = 0; paramIdx < data_.size(); ++paramIdx)
   {
      magic_enum::enum_for_each<musicDevice::sound::ParameterAttr>(
          [paramIdx, this](auto parameterAttr) {
             const float val = musicDevice::sound::getParameterData(
                 data_[paramIdx], parameterAttr);
             if (val != 0.0)
             {
                emitDataChangedUI(paramIdx, parameterAttr, val);
             }
          });
   }
}

inline void ParameterCache::syncRtToNonRt()
{
   std::ranges::copy(data_, nonRtBackupData_.begin());
}

inline void ParameterCache::syncNonRtToRt()
{
   std::ranges::copy(nonRtBackupData_, data_.begin());
}

}   // namespace base::instruments

#endif