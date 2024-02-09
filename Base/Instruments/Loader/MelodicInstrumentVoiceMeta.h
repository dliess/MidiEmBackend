#ifndef INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_LOADER_META
#define INSTRUMENTS_MELODIC_INSTRUMENTS_VOICE_LOADER_META

#include <JsonCast.h>

#include "MelodicInstrumentVoice.h"
#include "Meta.h"

namespace base::instruments::loader
{
inline void to_json(nlohmann::json& j, const MelodicVoice::Components& data)
{
   j = nlohmann::json::array();
   for (const auto& component : data)
   {
      if (component)
      {
         j.push_back(component.value());
      }
      else
      {
         j.push_back(nullptr);
      }
   }
}

inline void from_json(const nlohmann::json& j, MelodicVoice::Components& data)
{
   if (j.is_array() && j.size() == data.size())
   {
      for (std::size_t i = 0; i < j.size(); ++i)
      {
         if (j[i].is_null())
         {
            data[i] = std::nullopt;
         }
         else
         {
            data[i] = j[i].get<Component>();
         }
      }
   }
}

}   // namespace base::instruments::loader

namespace meta
{
template <> inline auto registerMembers<base::instruments::loader::MelodicVoice>()
{
   return members(
       member("components", &base::instruments::loader::MelodicVoice::components));
}

}   // namespace meta

#endif
