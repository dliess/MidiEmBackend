#ifndef BASE_SESSION_TRACK_META_H
#define BASE_SESSION_TRACK_META_H

#include "Track.h"
#include <nlohmann/json.hpp>
#include "JsonCast.h"
#include "Meta.h"

namespace base::session
{

inline
void to_json(nlohmann::json& j, const Track& p) {
    j = nlohmann::json{{"name", p.name()}};
}

}   // namespace base::session

#endif