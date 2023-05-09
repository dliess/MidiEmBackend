#include <gtest/gtest.h>
#include <string>
#include <optional>

#include "JsonCast.h"

struct Person
{
   std::string        name;
   int                age;
   std::optional<int> weight;
   auto operator<=>(const Person&) const = default;
};

namespace meta
{

template <>
inline auto registerMembers<Person>()
{
   return members(
      member("name", &Person::name),
      member("age", &Person::age),
      member("weight", &Person::weight)
   );
}

} // namespace meta

TEST(JsonParseOptionalsTest, OptionalsUnset) {
   Person person({"Dodo", 39});
   nlohmann::json jPerson(person);
   auto newPerson = jPerson.get<Person>();
   ASSERT_EQ(person, newPerson);
}

TEST(JsonParseOptionalsTest, OptionalsSet) {
   Person person({"Dodo", 39});
   person.weight.emplace(98);
   nlohmann::json jPerson(person);
   auto newPerson = jPerson.get<Person>();
   ASSERT_EQ(person, newPerson);
}

TEST(JsonParseOptionalsTest, DeserializeNonExistingNonOptional) {
   auto j = nlohmann::json::parse("{\"name\":\"Dodo\",\"weight\":98}");
   ASSERT_THROW(auto person = j.get<Person>(), std::exception);
}

TEST(JsonParseOptionalsTest, DeserializeNonExistingOptional) {
   auto j = nlohmann::json::parse("{\"name\":\"Dodo\",\"age\":11}");
   ASSERT_NO_THROW(auto person = j.get<Person>());
}