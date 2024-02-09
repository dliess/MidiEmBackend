#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_RT_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_RT_INL

#include "InstrumentComponent.h"

namespace base::instruments::rt
{

inline Component::Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            std::shared_ptr<ParameterCache> pParameterCache,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIdx, int noteOffset) noexcept :
    m_pSoundDevice(pSoundDevice),
    m_pParameterCache(std::move(pParameterCache)),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_sdVoiceIdx(sdVoiceIdx),
    m_noteOffset(noteOffset)
{
}

inline void Component::setParameterCache(std::shared_ptr<ParameterCache> p)
{
   m_pParameterCache = p;
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
   if(m_noteOffset != noteOffset)
   {
      m_noteOffset = noteOffset;
      emitNoteOffsetChanged(m_noteOffset);
   }
}

inline void Component::setAmp(float amp, float prevAmp)
{
   if(m_amp != amp)
   {
      m_amp = amp;
      emitAmpChanged(m_amp);
   }
   if(m_pSoundDevice)
   {
      m_pSoundDevice->setAmp(m_sdVoiceIdx, prevAmp * m_amp);
   }
}

inline bool Component::operator==(const Component& rhs) const
{
   return m_soundDeviceId == rhs.m_soundDeviceId &&
          m_sdVoiceIdx == rhs.m_sdVoiceIdx &&
          m_noteOffset == rhs.m_noteOffset;
}

inline void Component::updateParameterUI() const
{
   if (m_pParameterCache)
   {
      m_pParameterCache->updateParameterUI();
   }
}

inline ParameterCache* Component::parameterCache()
{
   return m_pParameterCache.get();
}

inline ParameterCache* Component::parameterCache() const
{
   return m_pParameterCache.get();
}

}   // namespace base::instruments::rt

#endif
