#ifndef BASE_SESSION_TRACKS_META_H
#define BASE_SESSION_TRACKS_META_H

#include <nlohmann/json.hpp>

#include "JsonCast.h"
#include "Meta.h"
#include "Track.h"

namespace base::session
{
inline void to_json(nlohmann::json& j, const Tracks& tracks)
{
   j["tracks"] = nlohmann::json::array();
   for (auto& track : tracks.m_tracks)
   {
      auto jN = nlohmann::json(track);
      j["tracks"].push_back(jN);
   }
}

}   // namespace base::session

#endif