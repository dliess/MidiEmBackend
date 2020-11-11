#include <gtest/gtest.h>

#include "ControllerHandler.h"
#include "MusicDevicePluginHandler.h"
#include "ControllerSection.h"

using namespace base::musicDevice;

TEST(ControllerHandlerTest, test)
{
   description::controller::Section controllerSection;
   PluginHandler pluginHandler("somewhere");
   controller::ControllerHandler controllerHandler("name", pluginHandler, controllerSection);
   controller::ControllerHandler other(std::move(controllerHandler));
}