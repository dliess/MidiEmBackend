#ifndef INSTRUMENTS_INSTRUMENT_VOICE_INL
#define INSTRUMENTS_INSTRUMENT_VOICE_INL

#include "InstrumentVoice.h"

namespace base::instruments
{
inline Voice::Voice(musicDevice::sound::SoundHandler* pSoundDevice,
                    std::shared_ptr<ParameterCache> pParameterCache,
                    musicDevice::MusicDeviceId soundDeviceId, int voiceIndex,
                    int noteOffset) noexcept :
    m_pSoundDevice(pSoundDevice),
    m_pParameterCache(std::move(pParameterCache)),
    m_soundDeviceId(std::move(soundDeviceId)),
    m_voiceIndex(voiceIndex),
    m_noteOffset(noteOffset)
{
   assert(m_pParameterCache);
}

inline const musicDevice::MusicDeviceId& Voice::soundDeviceId() const
{
   return m_soundDeviceId;
}

inline const musicDevice::sound::SoundHandler* Voice::pSoundDevice() const
{
   return m_pSoundDevice;
}

inline void Voice::setSoundDevicePtr(musicDevice::sound::SoundHandler* ptr)
{
   m_pSoundDevice = ptr;
}

inline void Voice::setNoteOffset(int noteOffset) { m_noteOffset = noteOffset; }

inline bool Voice::operator==(const Voice& rhs) const
{
   return m_soundDeviceId == rhs.m_soundDeviceId &&
          m_voiceIndex == rhs.m_voiceIndex && m_noteOffset == rhs.m_noteOffset;
}

}   // namespace base::instruments

#endif
