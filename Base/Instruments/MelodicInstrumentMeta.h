#ifndef MELODIC_INSTRUMENT_META_H
#define MELODIC_INSTRUMENT_META_H

#include <nlohmann/json.hpp>

namespace base::instruments
{

inline
void to_json(nlohmann::json& j, const MelodicInstrument& instr)
{
    j["id"] = instr.m_uuid;
    j["name"] = instr.m_name;
    j["voices"] = instr.m_voices;
    j["parameterCaches"] = nlohmann::json::array();
    instr.forEachLeadComponentExt([&j](const auto& component, int componentIdx){
        while(componentIdx > j["parameterCaches"].size()) {
            j["parameterCaches"].push_back(nullptr);
        }
        j["parameterCaches"].push_back(*component.parameterCache());
    });
}

inline
void from_json(const nlohmann::json& j, MelodicInstrument& instr)
{
    j["id"].get_to(instr.m_uuid);
    j["name"].get_to(instr.m_name);
    j["voices"].get_to(instr.m_voices);
    const auto& jPCaches = j["parameterCaches"];
    assert(jPCaches.size() == MelodicVoice::NUM_MAX_COMPONENTS_PER_VOICE);
    for(int componentIdx = 0; componentIdx < jPCaches.size(); ++componentIdx) {
        if(jPCaches[componentIdx] != nullptr) {
            auto parameterCache = std::make_shared<ParameterCache>(jPCaches.size());
            jPCaches[componentIdx].get_to(*parameterCache);
            parameterCache->syncBackupToRt();
            instr.forEachComponent(componentIdx, [&parameterCache](auto& component){
                component.setParameterCache(parameterCache);
            });
        }
    }
}


} // namespace base::instruments

#endif // MELODIC_INSTRUMENT_META_H