#ifndef BASE_SESSION_CLIP_META_H
#define BASE_SESSION_CLIP_META_H

#include "Clip.h"
#include <nlohmann/json.hpp>
#include "JsonCast.h"
#include "Meta.h"

namespace base::session
{

inline
void to_json(nlohmann::json& j, const Clip& p) {
    j["name"] = p.m_name;
    j["sequenceLength"] = p.m_sequenceLength;
    j["sequence"] = nlohmann::json::array();
    for(auto& it : p.m_noteEvents)
    {
        auto jN = nlohmann::json(it.second);
        j["sequence"].push_back(jN);
    }
}

}   // namespace base::session

#endif