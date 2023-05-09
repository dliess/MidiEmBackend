#pragma once

#include <Meta.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "StringCast.h"
#include "mpark/variant.hpp"

namespace meta
{
template <typename T> struct is_optional : std::false_type
{
};

template <typename T> struct is_optional<std::optional<T>> : std::true_type
{
};

template <typename T> concept is_variant = requires
{
   typename mpark::variant_size<T>::type;
   typename mpark::variant_alternative<0, T>::type;
};
static_assert(is_variant<mpark::variant<int, double>>);
static_assert(!is_variant<int>);

template<typename T>
concept TypeIsHandledByMeta = (meta::isRegistered<T>()) || meta::is_variant<T> ||
    meta::is_optional<T>::value;
}   // namespace meta

namespace nlohmann
{
template <typename T>
    requires meta::TypeIsHandledByMeta<T> 
    void to_json(nlohmann::json& j, const T& obj);

template <typename T>
    requires meta::TypeIsHandledByMeta<T> 
    void from_json(const nlohmann::json& j, T& obj);
}   // namespace nlohmann

namespace meta
{
/////////////////// SERIALIZATION
template <typename Class,
          typename = std::enable_if_t<meta::isRegistered<Class>()>>
nlohmann::json serialize(const Class& obj);

template <typename Class,
          typename = std::enable_if_t<!meta::isRegistered<Class>()>,
          typename = void>
nlohmann::json serialize(const Class& obj);

/*
template <typename Class> nlohmann::json serialize_basic(const Class& obj);

// specialization for std::vector
template <typename T> nlohmann::json serialize_basic(const std::vector<T>& obj);

// specialization for std::unodered_map
template <typename K, typename V,
          typename std::enable_if<
              std::is_convertible<K, std::string>::value &&
              std::is_same<decltype(std::to_string(std::declval<K>())),
                           std::string>::value>::type* = nullptr>
nlohmann::json serialize_basic(const std::unordered_map<K, V>& obj);
*/
// specialization for mpark::variant
template <typename... T>
nlohmann::json serialize_basic(const mpark::variant<T...>& obj);
inline nlohmann::json serialize_basic(const mpark::monostate& obj);

/////////////////// DESERIALIZATION
//

template <typename Class,
          typename = std::enable_if_t<meta::isRegistered<Class>()>>
void deserialize(Class& obj, const nlohmann::json& object);

template <typename Class,
          typename = std::enable_if_t<!meta::isRegistered<Class>()>,
          typename = void>
void deserialize(Class& obj, const nlohmann::json& object);

template <typename Class>
void deserialize_basic(Class& obj, const nlohmann::json& object);

// specialization for std::vector
template <typename T>
void deserialize_basic(std::vector<T>& obj, const nlohmann::json& object);

// specialization for std::unodered_map
template <typename K, typename V,
          typename std::enable_if<
              std::is_convertible<K, std::string>::value &&
              std::is_same<decltype(std::to_string(std::declval<K>())),
                           std::string>::value>::type* = nullptr>
void deserialize_basic(std::unordered_map<K, V>& obj,
                       const nlohmann::json& object);

// specialization for mpark::variant
template <typename... T>
void deserialize_basic(mpark::variant<T...>& ret, const nlohmann::json& object);
inline void deserialize_basic(mpark::monostate& ret,
                              const nlohmann::json& object);

}   // namespace meta

#include "JsonCast.inl"