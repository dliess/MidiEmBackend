#include <gtest/gtest.h>

#include "MusicDevicePluginHandler.h"

using namespace base::musicDevice;

TEST(PluginHandlerTest, test)
{
   PluginHandler pluginHandler("somewhere");
   PluginHandler other(std::move(pluginHandler));
}