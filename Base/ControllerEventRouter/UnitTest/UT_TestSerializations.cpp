#include <gtest/gtest.h>

#include "ControllerEventRouterLoader.h"
#include "Meta.h"

using namespace base::musicDevice;

TEST(TestSerializations, test)
{
   controller::loader::EventIdExt from;
   from.mdId = MusicDeviceId("type", "port");
   from.eventId = controller::EventId{3, controller::WidgetCoord{5, 7}, 5, 7};
   const auto j = meta::serialize(from);

   controller::loader::EventDestinationL to;
   to.mdId = MusicDeviceId("type", "port");
   to.voiceIdx = 2;
   to.controlType = controller::EventDestination::ParameterBase{5, controller::ParameterDestination::LFOWaveform};
   const auto k = meta::serialize(to);

   const auto l = meta::serialize(controller::loader::EventRoutes::MapEntry{from, to});
}