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

// inline const musicDevice::MusicDeviceId& Component::soundDeviceId() const
// {
//    return m_soundDeviceId;
// }
//
// inline const musicDevice::sound::SoundHandler* Component::pSoundDevice() const
// {
//    return m_pSoundDevice;
// }

inline void KitComponent::setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr)
{
   m_pSoundDevice = ptr;
}

inline void KitComponent::setNoteOffset(int noteOffset)
{
   if(m_noteOffset != noteOffset)
   {
      m_noteOffset = noteOffset;
      emitNoteOffsetChanged(m_noteOffset);
   }
}

inline void KitComponent::setAmp(float amp, float prevAmp)
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

inline bool KitComponent::operator==(const KitComponent& rhs) const
{
   return m_soundDeviceId == rhs.m_soundDeviceId &&
          m_sdVoiceIdx == rhs.m_sdVoiceIdx &&
          m_noteOffset == rhs.m_noteOffset;
}

}   // namespace base::instruments::loader

#endif
