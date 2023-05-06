#include <gtest/gtest.h>

#include "JsonCast.h"

namespace base
{
struct Person
{
   std::string name;
   int age;
   int weight;
};
}   // namespace base

namespace meta
{

template <>
inline auto registerMembers<base::Person>()
{
   return members(
      member("name", &base::Person::name),
      member("age", &base::Person::age),
      member("weight", &base::Person::weight)
   );
}

} // namespace meta



TEST(JsonTest, objectSerializeDeserialize)
{
   base::Person person{"George", 40, 88};
   nlohmann::json j(person);
}
