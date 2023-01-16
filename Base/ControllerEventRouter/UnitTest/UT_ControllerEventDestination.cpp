#include <gtest/gtest.h>
#include <iostream>

#include "ControllerEventDestination.h"
#include "Meta.h"

using namespace base::eventRouter;

struct UUIDCreator : public util::Identifiable {};

TEST(TestSerializations, test)
{
   EventDestination eventDestination;
   eventDestination.endpoint.emplace<EventDestination::DrumKit>(UUIDCreator().id(), 3, 5);
   const auto j = meta::serialize(eventDestination);
   std::cout << j.dump() << "\n";
}