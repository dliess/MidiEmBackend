#ifndef INSTRUMENTS_META_H
#define INSTRUMENTS_META_H

#include "Meta.h"

template<>
inline void to_json<base::instruments::KitInstruments>(
   nlohmann::json& j,
   const base::instruments::KitInstruments& obj)
{
   j = meta::serialize(*static_cast<const base::instruments::KitInstruments::Super*>(&obj));
}

template<>
inline void from_json<base::instruments::KitInstruments>(
   const nlohmann::json& j,
   base::instruments::KitInstruments& obj)
{
   obj = meta::deserialize<base::instruments::KitInstruments::Super>(j);
}

template<>
inline void to_json<base::instruments::MelodicInstruments>(
   nlohmann::json& j,
   const base::instruments::MelodicInstruments& obj)
{
   j = meta::serialize(*static_cast<const base::instruments::MelodicInstruments::Super*>(&obj));
}

template<>
inline void from_json<base::instruments::MelodicInstruments>(
   const nlohmann::json& j,
   base::instruments::MelodicInstruments& obj)
{
   obj = meta::deserialize<base::instruments::MelodicInstruments::Super>(j);
}

template<>
inline void to_json<std::shared_ptr<base::instruments::KitInstrument>>(
   nlohmann::json& j,
   const std::shared_ptr<base::instruments::KitInstrument>& obj)
{
   if (obj)
   {
      j = meta::serialize(*obj);
   }
}

template<>
inline void from_json<std::shared_ptr<base::instruments::KitInstrument>>(
   const nlohmann::json& j,
   std::shared_ptr<base::instruments::KitInstrument>& obj)
{
   obj = std::make_shared<base::instruments::KitInstrument>(
      meta::deserialize<base::instruments::KitInstrument>(j));
}

template<>
inline void to_json<std::shared_ptr<base::instruments::MelodicInstrument>>(
   nlohmann::json& j,
   const std::shared_ptr<base::instruments::MelodicInstrument>& obj)
{
   if (obj)
   {
      j = meta::serialize(*obj);
   }
}

template<>
inline void from_json<std::shared_ptr<base::instruments::MelodicInstrument>>(
   const nlohmann::json& j,
   std::shared_ptr<base::instruments::MelodicInstrument>& obj)
{
   obj = std::make_shared<base::instruments::MelodicInstrument>(
      meta::deserialize<base::instruments::MelodicInstrument>(j));
}

namespace meta
{
template<>
inline auto registerMembers<base::instruments::Instruments::Data>()
{
   return members(
      member("kitInstruments", &base::instruments::Instruments::Data::kitInstruments),
      member("melodicInstruments",
             &base::instruments::Instruments::Data::melodicInstruments));
}

} // namespace meta

#endif