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
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   musicDevice::sound::setParameterData(data_.at(parameterIdx), parameterAttr, value);
   dirtyFlags_.set(parameterIdx, parameterAttr);
}

inline void ParameterCache::setParameterBackup(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   musicDevice::sound::setParameterData(nonRtBackupData_.at(parameterIdx), parameterAttr, value);
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