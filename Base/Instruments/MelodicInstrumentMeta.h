#ifndef MELODIC_INSTRUMENT_META_H
#define MELODIC_INSTRUMENT_META_H

#include "Meta.h"

namespace meta
{

template <>
inline auto registerMembers<base::instruments::MelodicInstrumentVoice>()
{
    return members(
        member("soundDeviceId", &base::instruments::MelodicInstrumentVoice::soundDeviceId),
        member("voiceIndex", &base::instruments::MelodicInstrumentVoice::voiceIndex)
    );
}

template <>
inline auto registerMembers<base::instruments::MelodicInstrument>()
{
    return members(
        member("id", &base::instruments::MelodicInstrument::m_uuid),
        member("name", &base::instruments::MelodicInstrument::m_name),
        member("voices", &base::instruments::MelodicInstrument::m_voices)
    );
}

} // namespace meta

#endif // MELODIC_INSTRUMENT_META_H