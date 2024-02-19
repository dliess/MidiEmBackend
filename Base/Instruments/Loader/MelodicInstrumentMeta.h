#ifndef MELODIC_INSTRUMENT_LOADER_META_H
#define MELODIC_INSTRUMENT_LOADER_META_H

#include <nlohmann/json.hpp>

#include <Meta.h>
#include <JsonCast.h>
#include "MelodicInstrument.h"


namespace meta
{



template <> inline auto registerMembers<base::instruments::loader::MelodicInstrument::Voice::Component>()
{
   return members(
       member("soundDeviceId", &base::instruments::loader::MelodicInstrument::Voice::Component::soundDeviceId),
       member("sdVoiceIdx", &base::instruments::loader::MelodicInstrument::Voice::Component::sdVoiceIdx)
   );
}

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument::ParameterData::EngineId>()
{
   return members(member("mdName", &base::instruments::loader::MelodicInstrument::ParameterData::EngineId::mdName),
                  member("engineIdx", &base::instruments::loader::MelodicInstrument::ParameterData::EngineId::engineIdx)
   );
}

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument::ParameterData>()
{
   return members(member("deviceParameters", &base::instruments::loader::MelodicInstrument::ParameterData::deviceParameters),
                  member("noteOffset", &base::instruments::loader::MelodicInstrument::ParameterData::noteOffset),
                  member("amp", &base::instruments::loader::MelodicInstrument::ParameterData::amp)
   );
}

}  // namespace meta

namespace base::instruments::loader
{

template <typename T, size_t N>
inline void to_json(nlohmann::json& j, const std::array<std::optional<T>, N>& data)
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

template <typename T, size_t N>
inline void from_json(const nlohmann::json& j, std::array<std::optional<T>, N>& data)
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
            data[i] = j[i].get<T>();
         }
      }
   }
}
/*
inline void to_json(nlohmann::json& j, const MelodicInstrument::Voice& data)
{
   j = nlohmann::json::object();
   auto jArr = nlohmann::json::array();
   for (const auto& component : data.components)
   {
      if (component)
      {
         jArr.push_back(component.value());
      }
      else
      {
         jArr.push_back(nullptr);
      }
   }
   j["components"] = jArr;
}

inline void from_json(const nlohmann::json& j, MelodicInstrument::Voice& data)
{
   if (j.is_object() && j.contains("components"))
   {
      auto jArr = j.at("components");
      if (jArr.is_array() && jArr.size() == data.components.size())
      {
         for (std::size_t i = 0; i < jArr.size(); ++i)
         {
            if (jArr[i].is_null())
            {
               data.components[i] = std::nullopt;
            }
            else
            {
               data.components[i] = jArr[i].get<MelodicInstrument::Voice::Component>();
            }
         }
      }
   }
}
*/
}   // namespace base::instruments::loader

namespace meta
{

template <> inline auto registerMembers<base::instruments::loader::MelodicInstrument::Voice>()
{
   return members(
       member("components", &base::instruments::loader::MelodicInstrument::Voice::components));
}

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument>()
{
    return members(member("id", &base::instruments::loader::MelodicInstrument::m_uuid),
                   member("name", &base::instruments::loader::MelodicInstrument::m_name),
                   member("voices", &base::instruments::loader::MelodicInstrument::m_voices),
                   member("parameters", &base::instruments::loader::MelodicInstrument::m_parameters)
                   );
}

}   // namespace meta

#endif // MELODIC_INSTRUMENT_LOADER_META_H
