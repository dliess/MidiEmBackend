#include <gtest/gtest.h>
#include <iostream>

#include "ControllerEventDestination.h"
#include "Meta.h"

using namespace base::eventRouter;

struct UUIDCreator : public util::Identifiable {};

TEST(ControllerEventDestination, SerializationTest)
{
   EventDestination eventDestination;
   eventDestination.endpoint.emplace<EventDestination::DrumKit>(UUIDCreator().id(), 3, 5);
   const auto j = nlohmann::json(eventDestination);
   const auto jString = j.dump();
   nlohmann::json jRet = nlohmann::json::parse(jString);
   auto retData     = jRet.get<EventDestination>();
}