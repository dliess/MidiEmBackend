#ifndef MELODIC_INSTRUMENT_LOADER_META_H
#define MELODIC_INSTRUMENT_LOADER_META_H

#include <nlohmann/json.hpp>

#include <Meta.h>
#include <JsonCast.h>

/*
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
*/
namespace meta
{

template<>
inline auto registerMembers<base::instruments::loader::MelodicInstrument>()
{
    return members(member("id", &base::instruments::loader::MelodicInstrument::m_uuid),
                   member("name", &base::instruments::loader::MelodicInstrument::m_name),
                   member("voices", &base::instruments::loader::MelodicInstrument::m_voices),
                   member("voices", &base::instruments::loader::MelodicInstrument::m_parameters)
                   );
}

template <> inline auto registerMembers<base::instruments::loader::MelodicInstrument::Voice>()
{
   return members(
       member("components", &base::instruments::loader::MelodicInstrument::Voice::components));
}

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

}   // namespace meta

#endif // MELODIC_INSTRUMENT_LOADER_META_H
