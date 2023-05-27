#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_INL

#include "InstrumentComponent.h"

namespace base::instruments
{
inline const Component::ParameterCache::ParameterData&
Component::ParameterCache::at(std::size_t pos) const
{
   return data_.at(pos);
}

inline void Component::ParameterCache::setParameter(
    std::size_t index, musicDevice::sound::ParameterAttr parameterAttr,
    float value)
{
   musicDevice::sound::setParameterData(data_.at(index), parameterAttr, value);
   dirtyFlags_.set(index, parameterAttr);
}

inline void Component::ParameterCache::updateParameterUI()
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

inline void Component::ParameterCache::emiAllNonNullParameters()
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

inline Component::Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            std::shared_ptr<ParameterCache> pParameterCache,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIndex, int noteOffset) noexcept :
    m_pSoundDevice(pSoundDevice),
    m_pParameterCache(std::move(pParameterCache)),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_sdVoiceIndex(sdVoiceIndex),
    m_noteOffset(noteOffset)
{
}

inline const musicDevice::MusicDeviceId& Component::soundDeviceId() const
{
   return m_soundDeviceId;
}

inline const musicDevice::sound::SoundHandler* Component::pSoundDevice() const
{
   return m_pSoundDevice;
}

inline void Component::setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr)
{
   m_pSoundDevice = ptr;
}

inline void Component::setNoteOffset(int noteOffset)
{
   m_noteOffset = noteOffset;
}

inline bool Component::operator==(const Component& rhs) const
{
   return m_soundDeviceId == rhs.m_soundDeviceId &&
          m_sdVoiceIndex == rhs.m_sdVoiceIndex &&
          m_noteOffset == rhs.m_noteOffset;
}

inline void Component::updateParameterUI() const
{
   if (m_pParameterCache)
   {
      m_pParameterCache->updateParameterUI();
   }
}

inline Component::ParameterCache* Component::parameterCache()
{
   return m_pParameterCache.get();
}

inline Component::ParameterCache* Component::parameterCache() const
{
   return m_pParameterCache.get();
}

}   // namespace base::instruments

#endif
