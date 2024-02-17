#ifndef KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_INL
#define KIT_INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_INL

#include "KitInstrumentComponent.h"

namespace base::instruments::loader
{

inline KitComponent::KitComponent(musicDevice::sound::SoundHandler* pSoundDevice,
                            std::size_t numParameter,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIdx, int noteOffset) noexcept :
    m_parameterData(numParameter),
    m_pSoundDevice(pSoundDevice),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_sdVoiceIdx(sdVoiceIdx),
    m_noteOffset(noteOffset)
{
}

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

inline bool KitComponent::setAmp(float amp)
{
   if(m_amp != amp)
   {
      m_amp = amp;
      return true;
   }
   return false;
}

inline bool KitComponent::operator==(const KitComponent& rhs) const
{
   return m_soundDeviceId == rhs.m_soundDeviceId &&
          m_sdVoiceIdx == rhs.m_sdVoiceIdx &&
          m_noteOffset == rhs.m_noteOffset;
}

}   // namespace base::instruments::loader

#endif
