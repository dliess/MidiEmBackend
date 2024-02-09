#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_RT_INL
#define BASE_INSTRUMENTS_PARAMETER_CACHE_RT_INL

#include "InstrumentsParameterCache.h"

namespace base::instruments::rt
{
inline ParameterCache::ParameterCache(size_t size) :
    data_(size),
    valueModifier_(size),
    nonRtBackupData_(size),
    dirtyFlags_(size),
    dontOverwriteOnNextNoteOn_(size)
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
   musicDevice::sound::setParameterData(data_.at(parameterIdx), parameterAttr,
                                        value);
   dirtyFlags_.set(parameterIdx, parameterAttr);
}

inline float ParameterCache::getParameter(
    std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return musicDevice::sound::getParameterData(data_.at(parameterIdx),
                                               parameterAttr);
}

inline float ParameterCache::getModifiedParameterValue(
    std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   const auto commanded = getParameter(parameterIdx, parameterAttr);
   const auto& modifier = musicDevice::sound::getParameterDataConstRef(
       valueModifier_.at(parameterIdx), parameterAttr);

   return calculateModifiedValue(commanded, modifier);
}

inline void ParameterCache::clearModifier(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr)
{
   auto& modifier = musicDevice::sound::getParameterDataRef(
       valueModifier_.at(parameterIdx), parameterAttr);
   modifier.clear();
}

inline void ParameterCache::applyModifier(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float destination, float intensity)
{
   auto& modifier = musicDevice::sound::getParameterDataRef(
       valueModifier_.at(parameterIdx), parameterAttr);
   modifier.addAbsoluteModifier(destination, intensity);
}

inline void ParameterCache::setParameterBackup(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   musicDevice::sound::setParameterData(nonRtBackupData_.at(parameterIdx),
                                        parameterAttr, value);
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

inline void ParameterCache::syncBackupToRt()
{
   std::ranges::copy(nonRtBackupData_, data_.begin());
}

inline void ParameterCache::dontOverwriteOnNextNoteOn(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr)
{
   dontOverwriteOnNextNoteOn_.set(parameterIdx, parameterAttr);
}

inline bool ParameterCache::shouldBeOverwritten(
    std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return !dontOverwriteOnNextNoteOn_.contains(parameterIdx, parameterAttr);
}

inline void ParameterCache::clearOverwriteList()
{
   dontOverwriteOnNextNoteOn_.reset();
}

}   // namespace base::instruments::rt

#endif
