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

template <typename T> concept IsVariant = requires
{
   typename mpark::variant_size<T>::type;
   typename mpark::variant_alternative<0, T>::type;
};
static_assert(IsVariant<mpark::variant<int, double>>);
static_assert(!IsVariant<int>);
static_assert(!IsVariant<std::array<unsigned char, 16>>);

template<typename T>
concept IsRegistered = (isRegistered<T>());
template<typename T>
concept IsNotRegistered = (!isRegistered<T>());
template<typename T>
concept IsOptional = is_optional<T>::value;
template<typename T>
concept IsNotOptional = !IsOptional<T>;

template<typename T>
concept TypeIsHandledByMeta = IsRegistered<T> || IsVariant<T>;

static_assert(!TypeIsHandledByMeta<std::array<unsigned char, 16>>);
 
}   // namespace meta

namespace nlohmann
{
template <meta::TypeIsHandledByMeta T>
void to_json(nlohmann::json& j, const T& obj);

template <meta::TypeIsHandledByMeta T>
void from_json(const nlohmann::json& j, T& obj);
}   // namespace nlohmann

namespace meta
{
/////////////////// SERIALIZATION
template <IsRegistered T>
nlohmann::json serialize(const T& obj);

template <IsNotRegistered T>
nlohmann::json serialize(const T& obj);

// specialization for mpark::variant
template <typename... T>
nlohmann::json serialize_variant(const mpark::variant<T...>& obj);
inline nlohmann::json serialize_variant(const mpark::monostate& obj);

/////////////////// DESERIALIZATION
//
template <IsRegistered T>
void deserialize(T& obj, const nlohmann::json& object);

template <IsNotRegistered T>
void deserialize(T& obj, const nlohmann::json& object);

// specialization for mpark::variant
template <typename... T>
void deserialize_variant(mpark::variant<T...>& ret, const nlohmann::json& object);
inline void deserialize_variant(mpark::monostate& ret,
                              const nlohmann::json& object);

}   // namespace meta

#include "JsonCast.inl"