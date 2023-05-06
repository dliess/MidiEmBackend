#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

namespace base
{
struct Person
{
   std::string name;
   int age;
   int weight;
};

void to_json(nlohmann::json& j, const Person& p)
{
   j = nlohmann::json{{"name", p.name}, {"age", p.age}, {"weight", p.weight}};
}

void from_json(const nlohmann::json& j, Person& p)
{
   j.at("name").get_to(p.name);
   j.at("age").get_to(p.age);
   j.at("weight").get_to(p.weight);
}

}   // namespace base

TEST(JsonTest, objectSerializeDeserialize)
{
   base::Person person{"George", 40, 88};
   nlohmann::json j(person);
}
