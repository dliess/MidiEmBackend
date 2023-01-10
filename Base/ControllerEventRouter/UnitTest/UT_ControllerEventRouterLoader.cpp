#include <gtest/gtest.h>

#include "ControllerEventDestination.h"
#include "ControllerEventRouterLoader.h"
#include "MusicDeviceId.h"
#include "Meta.h"

using namespace base;

TEST(TestSerializations, test)
{
   eventRouter::loader::EventIdExt from;
   from.mdId = musicDevice::MusicDeviceId("type", "port");
   from.eventId = musicDevice::controller::EventId{3, musicDevice::controller::WidgetCoord{5, 7}, 5, 7};
   const auto j = meta::serialize(from);

   eventRouter::loader::EventDestinationL to;
   to.mdId = musicDevice::MusicDeviceId("type", "port");
   to.voiceIdx = 2;
   to.controlType = eventRouter::EventDestination::Parameter{5, eventRouter::ParameterDestination::LFOWaveform};
   const auto k = meta::serialize(to);

   const auto l = meta::serialize(eventRouter::loader::EventRoutes::MapEntry{from, to});
}