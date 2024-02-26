#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_INL
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_RT_INL

#include "KitInstrumentComponent.h"

namespace base::instruments::rt
{

inline KitComponent::KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIdx, int noteOffset) noexcept :
    m_pSoundDevice(pSoundDevice),
    m_parameterCache(pSoundDevice->description().engineBase(sdVoiceIdx)->parameters.size()),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_sdVoiceIdx(sdVoiceIdx),
    m_noteOffset(noteOffset)
{
}


// inline const musicDevice::MusicDeviceId& KitComponent::soundDeviceId() const
// {
//    return m_soundDeviceId;
// }
//
// inline const musicDevice::sound::SoundHandler* KitComponent::pSoundDevice() const
// {
//    return m_pSoundDevice;
// }

inline void KitComponent::setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr)
{
   m_pSoundDevice = ptr;
}

inline bool KitComponent::setNoteOffset(int noteOffset)
{
   if(m_noteOffset != noteOffset)
   {
      m_noteOffset = noteOffset;
      return true;
   }
   return false;
}

inline bool KitComponent::setAmp(float amp, float prevAmp)
{
   if(m_amp != amp)
   {
      m_amp = amp;
      if(m_pSoundDevice)
      {
         m_pSoundDevice->setAmp(m_sdVoiceIdx, prevAmp * m_amp);
      }
      return true;
   }
   return false;
}


inline void KitComponent::updateParameterUI()
{
   m_parameterCache.updateParameterUI();
}

}   // namespace base::instruments::rt

#endif
