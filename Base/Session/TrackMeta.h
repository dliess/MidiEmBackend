#ifndef BASE_SESSION_TRACK_META_H
#define BASE_SESSION_TRACK_META_H

#include <nlohmann/json.hpp>

#include "JsonCast.h"
#include "Meta.h"
#include "Track.h"

namespace base::session
{
inline void to_json(nlohmann::json& j, const Track& track)
{
   j["uuid"] = track.idView();
   j["name"] = track.m_name;
   if (track.m_instrumentUUID)
   {
      j["instrumentUuid"] = track.m_instrumentUUID.value();
   }
   if (track.m_activeClipIdx)
   {
      j["activeClipIdx"] = *track.m_activeClipIdx;
   }
   if (track.m_toStartClipIdx)
   {
      j["toStartClipIdx"] = *track.m_toStartClipIdx;
   }
   j["clips"] = nlohmann::json::array();
   for (auto& pClip : track.m_clips)
   {
      if (pClip)
      {
         auto jN = meta::serialize(*pClip);
         j["clips"].push_back(jN);
      }
      else
      {
         nlohmann::json j_null;
         j["clips"].push_back(j_null);
      }
   }
}

}   // namespace base::session

#endif