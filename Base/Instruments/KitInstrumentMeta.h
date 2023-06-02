#ifndef KIT_INSTRUMENT_META_H
#define KIT_INSTRUMENT_META_H

#include <nlohmann/json.hpp>

namespace base::instruments
{
inline void to_json(nlohmann::json& j, const KitInstrument& instr)
{
   j["id"]     = instr.m_uuid;
   j["name"]   = instr.m_name;
   j["voices"] = instr.m_voices;
   instr.forEachComponentExt(
       [&j](const auto& component, int voiceIdx, int componentIdx) {
          j["voices"][voiceIdx]["components"][componentIdx]["parameterCache"] =
              nlohmann::json(*component.parameterCache());
       });
}

inline void from_json(const nlohmann::json& j, KitInstrument& instr)
{
   j["id"].get_to(instr.m_uuid);
   j["name"].get_to(instr.m_name);
   j["voices"].get_to(instr.m_voices);
   instr.forEachComponentExt([&j](auto& component, int voiceIdx,
                                  int componentIdx) {
      auto& jParameterCache = j["voices"][voiceIdx]["components"][componentIdx]["parameterCache"];
      auto parameterCache = std::make_shared<ParameterCache>(jParameterCache.size());
      jParameterCache.get_to(*parameterCache);
      parameterCache->syncNonRtToRt();
      component.setParameterCache(parameterCache);
   });
}

}   // namespace base::instruments

#endif   // KIT_INSTRUMENT_META_H