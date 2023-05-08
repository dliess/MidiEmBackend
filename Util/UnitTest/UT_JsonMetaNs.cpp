#include <gtest/gtest.h>

#include "JsonCast.h"

namespace myns
{

struct Person
{
   std::string name;
   int age;
   int weight;
};
}   // namespace myns

namespace meta
{

template <>
inline auto registerMembers<myns::Person>()
{
   return members(
      member("name", &myns::Person::name),
      member("age", &myns::Person::age),
      member("weight", &myns::Person::weight)
   );
}

} // namespace meta

TEST(JsonMetaNsTest, objectSerializeDeserialize)
{
   myns::Person person{"George", 40, 88};
   nlohmann::json j(person);
}
