#include <fmt/format.h>

#include <iostream>

#include "JsonCast.h"

template <meta::TypeIsHandledByMeta T>
void nlohmann::to_json(nlohmann::json& j, const T& obj)
{
   j = meta::serialize(obj);
}

template <meta::TypeIsHandledByMeta T>
void nlohmann::from_json(const nlohmann::json& j, T& obj)
{
   meta::deserialize(obj, j);
}

namespace meta
{
template <class T> auto getClassNameOrIndex(int i) noexcept { return i; };

//////////////////// SOME HELPERS
template <std::size_t I = 0, typename... Tp, typename Fn>
inline void for_each_in_tuple(std::tuple<Tp...>& t, Fn&& func)
{
   if constexpr (I < sizeof...(Tp))
   {
      if (func(I, std::get<I>(t)))
      {
         for_each_in_tuple<I + 1, Tp...>(t, func);
      }
   }
}

/////////////////// SERIALIZATION

template <typename Class> class SerializerFunc
{
public:
   SerializerFunc(const Class& rObj, nlohmann::json& rValue) :
       m_rObj(rObj), m_rValue(rValue){};
   template <typename Member>
      requires IsOptional<get_member_type<Member>> 
   void operator()(Member& member)
   {
      if (member.canGetConstRef())
      {
         auto& rOptional = member.get(m_rObj);
         if (rOptional)
         {
            auto& valueName = m_rValue[member.getName()];
            valueName       = *member.get(m_rObj);
         }
      }
   }
   template <typename Member>
      requires IsNotOptional<get_member_type<Member>> 
   void operator()(Member& member)
   {
      auto& valueName = m_rValue[member.getName()];
      if (member.canGetConstRef())
      {
         valueName = member.get(m_rObj);
      }
      else if (member.hasGetter())
      {
         valueName =
             member.getCopy(m_rObj);   // passing copy as const ref, it's okay
      }
   }

private:
   const Class& m_rObj;
   nlohmann::json& m_rValue;
};

template <IsRegistered T> nlohmann::json serialize(const T& obj)
{
   nlohmann::json value;
   SerializerFunc<T> serializeFunc(obj, value);
   meta::doForAllMembers<T>(serializeFunc);
   return value;
}

template <IsNotRegistered T> nlohmann::json serialize(const T& obj)
{
   return serialize_variant(obj);
}

// specialization for mpark::variant
template <typename... T>
nlohmann::json serialize_variant(const mpark::variant<T...>& obj)
{
   nlohmann::json ret;
   std::tuple<T...> tuple;
   for_each_in_tuple(tuple, [&obj, &ret](int i, auto& typeHolder) {
      using Type = std::decay_t<decltype(typeHolder)>;
      auto p     = mpark::get_if<Type>(&obj);
      if (p)
      {
         ret          = meta::serialize(*p);
         ret[".type"] = getClassNameOrIndex<Type>(i);
      }
      return true;
   });
   return ret;
}

inline nlohmann::json serialize_variant(const mpark::monostate& obj)
{
   return nlohmann::json();
}

/////////////////// DESERIALIZATION
template <typename Class> class DeserializerFunc
{
public:
   DeserializerFunc(Class& rObj, const nlohmann::json& rValue) :
       m_rObj(rObj), m_rValue(rValue){};
   template <typename Member>
   requires IsNotOptional<get_member_type<Member>> void operator()(
       Member& member)
   {
      auto it = m_rValue.find(member.getName());
      if (it != m_rValue.end())
      {
         auto& objName = *it;
         if (!objName.is_null())
         {
            using MemberT = meta::get_member_type<decltype(member)>;
            if (member.hasSetter())
            {
               member.set(m_rObj, objName.template get<MemberT>());
            }
            else if (member.canGetRef())
            {
               member.getRef(m_rObj) = objName.template get<MemberT>();
            }
            else
            {
               throw std::runtime_error(
                   "Error: can't deserialize member because it's read only");
            }
         }
      }
      else
      {
         std::string errMsg("Error: JSon Map Key not found: ");
         errMsg.append(member.getName());
         throw std::runtime_error(errMsg);
      }
   }
   template <typename Member>
   requires IsOptional<get_member_type<Member>> void operator()(Member& member)
   {
      auto it = m_rValue.find(member.getName());
      if (it != m_rValue.end())
      {
         auto& objName = *it;
         using MemberT =
             typename meta::get_member_type<decltype(member)>::value_type;
         if (member.canGetRef())
         {
            member.getRef(m_rObj).emplace(objName.template get<MemberT>());
         }
         else
         {
            throw std::runtime_error(
                "Error: can't get reference to std::optional");
         }
      }
   }

private:
   Class& m_rObj;
   const nlohmann::json& m_rValue;
};

template <IsRegistered T> void deserialize(T& obj, const nlohmann::json& object)
{
   if (object.is_object())
   {
      DeserializerFunc<T> deserializerFunc(obj, object);
      meta::doForAllMembers<T>(deserializerFunc);
   }
   else
   {
      throw std::runtime_error(
          fmt::format("Error: can't deserialize from nlohmann::json to Class "
                      "because its not an object, it's an array. {}",
                      object.dump()));
   }
}

template <IsNotRegistered T>
void deserialize(T& obj, const nlohmann::json& object)
{
   deserialize_variant(obj, object);
}

// specialization for mpark::variant
template <typename... T>
void deserialize_variant(mpark::variant<T...>& ret, const nlohmann::json& object)
{
   std::tuple<T...> tuple;
   for_each_in_tuple(tuple, [&ret, &object](int i, auto& typeHolder) {
      using Type = std::decay_t<decltype(typeHolder)>;
      try
      {
         auto it = object.find(".type");
         if (it != object.end() && *it != getClassNameOrIndex<Type>(i))
         {
            throw std::runtime_error("");
         }
         if constexpr (std::is_same<Type, mpark::monostate>::value)
         {
            ret = mpark::monostate();
         }
         else
         {
            ret = object.get<Type>();
         }
         return false;   // we have the match
      }
      catch (std::runtime_error& e)
      {
         using FirstType =
             typename std::tuple_element<0, std::tuple<T...>>::type;
         using LastType = typename std::tuple_element<sizeof...(T) - 1,
                                                      std::tuple<T...>>::type;
         if (std::is_same<Type, LastType>::value)
         {
            throw std::runtime_error("Error: No Type in Variant matched");
         }
         return true;
      }
   });
}

inline void deserialize_variant(mpark::monostate& ret,
                              const nlohmann::json& object)
{
}

}   // namespace meta
