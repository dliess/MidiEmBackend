#ifndef MELODIC_INSTRUMENT_LOADER_INL
#define MELODIC_INSTRUMENT_LOADER_INL

#include "MelodicInstrument.h"

namespace base::instruments::loader
{


// template <typename Cb> void MelodicInstrument::forEachComponent(Cb&& cb)
// {
//    for (auto& voice : m_voices)
//    {
//       for (auto& component : voice.components)
//       {
//          if (component)
//          {
//             std::forward<Cb>(cb)(component.value());
//          }
//       }
//    }
// }
//
// template <typename Cb>
// void MelodicInstrument::forEachComponent(size_t componentIdx, Cb&& cb)
// {
//    for (auto& voice : m_voices)
//    {
//       if (voice.components.at(componentIdx))
//       {
//          std::forward<Cb>(cb)(voice.components.at(componentIdx).value());
//       }
//    }
// }
//
// template <typename Cb> void MelodicInstrument::forEachComponentExt(Cb&& cb)
// {
//    for (size_t voiceIdx = 0; voiceIdx < m_voices.size(); ++voiceIdx)
//    {
//       for (size_t componentIdx = 0;
//            componentIdx < m_voices[voiceIdx].components.size(); ++componentIdx)
//       {
//          if (m_voices[voiceIdx].components[componentIdx])
//          {
//             cb(m_voices[voiceIdx].components[componentIdx].value(), voiceIdx,
//                componentIdx);
//          }
//       }
//    }
// }
//
// template <typename Cb> void MelodicInstrument::forEachLeadComponent(Cb&& cb)
// {
//    for (size_t componentIdx = 0;
//         componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
//         ++componentIdx)
//    {
//       auto component = getFirstComponent(componentIdx);
//       if (component)
//       {
//          std::forward<Cb>(cb)(*component);
//       }
//    }
// }
//
// template <typename Cb>
// void MelodicInstrument::forEachLeadComponent(Cb&& cb) const
// {
//    for (size_t componentIdx = 0;
//         componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
//         ++componentIdx)
//    {
//       auto component = getFirstComponent(componentIdx);
//       if (component)
//       {
//          std::forward<Cb>(cb)(*component);
//       }
//    }
// }
//
// template <typename Cb>
// void MelodicInstrument::forEachLeadComponentExt(Cb&& cb) const
// {
//    for (size_t componentIdx = 0;
//         componentIdx < MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE;
//         ++componentIdx)
//    {
//       auto component = getFirstComponent(componentIdx);
//       if (component)
//       {
//          std::forward<Cb>(cb)(*component, componentIdx);
//       }
//    }
// }
inline
bool isSameInstrument(const MelodicInstrument& lhs,
                      const MelodicInstrument& rhs)
{
   return lhs.m_name == rhs.m_name;
}

}   // namespace base::instruments::loader

#endif   // MELODIC_INSTRUMENT_LOADER_INL
