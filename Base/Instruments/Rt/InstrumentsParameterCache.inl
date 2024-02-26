#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_RT_INL
#define BASE_INSTRUMENTS_PARAMETER_CACHE_RT_INL

#include "InstrumentsParameterCache.h"

namespace base::instruments::rt
{
inline ParameterCache::ParameterCache(size_t size) :
    data_(size),
    valueModifier_(size),
    dirtyFlags_(size),
    dontOverwriteOnNextNoteOn_(size)
{
}

inline std::size_t ParameterCache::size() const { return data_.size(); }

inline Ret<const ParameterCache::ParameterData*> ParameterCache::at(
    std::size_t pos) const
{
   return safe_at(data_, pos);
}

inline Void ParameterCache::setParameter(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   return safe_at(data_, parameterIdx).map(
      [&,this](auto data) {
         musicDevice::sound::setParameterData(*data, parameterAttr, value);
         dirtyFlags_.set(parameterIdx, parameterAttr);
      }
   );
}

inline Ret<float> ParameterCache::getParameter(
    std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return safe_at(data_, parameterIdx).map(
      [this,parameterAttr](auto data) {
         return musicDevice::sound::getParameterData(*data, parameterAttr);
      }
   );
}

inline Ret<float> ParameterCache::getModifiedParameterValue(
    std::size_t parameterIdx,
    musicDevice::sound::ParameterAttr parameterAttr) const
{
   return getParameter(parameterIdx, parameterAttr).and_then(
      [&,this](float commanded) {
         return safe_at(valueModifier_, parameterIdx).map(
            [&](auto modifier) -> float {
               return base::musicDevice::sound::calculateModifiedValue<float>(commanded, *modifier);
            }
         );
      }
   );
}

inline Void ParameterCache::clearModifier(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr)
{
   return safe_at(valueModifier_, parameterIdx).map(
      [&,this](auto modifier) {
         auto& modifierV = musicDevice::sound::getParameterDataRef(
             *modifier, parameterAttr);
         modifierV.clear();
      }
   );
}

inline Void ParameterCache::applyModifier(
    std::size_t parameterIdx, musicDevice::sound::ParameterAttr parameterAttr,
    float destination, float intensity)
{
   return safe_at(valueModifier_, parameterIdx).map(
      [&,this](auto modifier) {
         auto& modifierV = musicDevice::sound::getParameterDataRef(
             *modifier, parameterAttr);
         modifierV.addAbsoluteModifier(destination, intensity);
      }
   );
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

inline void ParameterCache::clearOverwriteList() const
{
   dontOverwriteOnNextNoteOn_.reset();
}

}   // namespace base::instruments::rt

#endif
