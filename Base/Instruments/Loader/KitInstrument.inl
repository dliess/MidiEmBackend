#ifndef INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL
#define INSTRUMENTS_KIT_INSTRUMENTS_LOADER_INL

#include "KitInstrument.h"

namespace base::instruments::loader
{

inline KitInstrument::Voice::Component::Component(const std::vector<musicDevice::description::sound::Parameter>& paramDescr,
                         musicDevice::MusicDeviceId soundDeviceId,
                         int sdVoiceIdx) noexcept
    : 
    paramDescr(&paramDescr),
    parameterData(paramDescr.size()),
    soundDeviceId(std::move(soundDeviceId)),
    sdVoiceIdx(sdVoiceIdx)
{
}

inline bool KitInstrument::Voice::Component::operator==(const Component& rhs) const
{
   return soundDeviceId == rhs.soundDeviceId &&
          sdVoiceIdx == rhs.sdVoiceIdx &&
          noteOffset == rhs.noteOffset;
}

inline bool isSameInstrument(const KitInstrument& lhs, const KitInstrument& rhs)
{
   return lhs.m_name == rhs.m_name && lhs.m_voices == rhs.m_voices;
}


}   // namespace base::instruments

#endif
