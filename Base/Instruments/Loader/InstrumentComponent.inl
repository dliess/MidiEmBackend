#ifndef INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_INL
#define INSTRUMENTS_INSTRUMENT_COMPONENT_LOADER_INL

#include "InstrumentComponent.h"

namespace base::instruments::loader
{

inline Component::Component(musicDevice::sound::SoundHandler* pSoundDevice,
                            musicDevice::MusicDeviceId soundDeviceId,
                            int sdVoiceIdx, int noteOffset) noexcept :
    m_pSoundDevice(pSoundDevice),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_sdVoiceIdx(sdVoiceIdx),
    m_noteOffset(noteOffset)
{
   // TODO create m_parameterData
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

}   // namespace base::instruments::loader

#endif
