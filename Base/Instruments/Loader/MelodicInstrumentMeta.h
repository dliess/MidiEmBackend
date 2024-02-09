#ifndef MELODIC_INSTRUMENT_LOADER_META_H
#define MELODIC_INSTRUMENT_LOADER_META_H

#include <nlohmann/json.hpp>

#include <Meta.h>
#include <JsonCast.h>

namespace meta {

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument>()
{
    return members(member("id", &base::instruments::loader::MelodicInstrument::m_uuid),
                   member("name", &base::instruments::loader::MelodicInstrument::m_name),
                   member("voices", &base::instruments::loader::MelodicInstrument::m_voices));
}

} // namespace meta


// namespace base::instruments::loader
// {
//
// inline
// void to_json(nlohmann::json& j, const MelodicInstrument& instr)
// {
//     j["id"] = instr.m_uuid;
//     j["name"] = instr.m_name;
//     j["voices"] = instr.m_voices;
//     j["parameterCaches"] = nlohmann::json::array();
//     instr.forEachLeadComponentExt([&j](const auto& component, int componentIdx){
//         while(componentIdx > j["parameterCaches"].size()) {
//             j["parameterCaches"].push_back(nullptr);
//         }
//         j["parameterCaches"].push_back(*component.parameterCache());
//     });
// }
//
// inline
// void from_json(const nlohmann::json& j, MelodicInstrument& instr)
// {
//     j["id"].get_to(instr.m_uuid);
//     j["name"].get_to(instr.m_name);
//     j["voices"].get_to(instr.m_voices);
//     const auto& jPCaches = j["parameterCaches"];
//     for(int componentIdx = 0; componentIdx < jPCaches.size(); ++componentIdx) {
//         if(jPCaches[componentIdx] != nullptr) {
//             const size_t numParams = jPCaches[componentIdx]["data"].size();
//             auto parameterCache = std::make_shared<ParameterCache>(numParams);
//             jPCaches[componentIdx].get_to(*parameterCache);
//             parameterCache->syncBackupToRt();
//             instr.forEachComponent(componentIdx, [parameterCache](auto& component){
//                 component.setParameterCache(parameterCache);
//             });
//         }
//     }
// }
//
// } // namespace base::instruments::loader

#endif // MELODIC_INSTRUMENT_LOADER_META_H
