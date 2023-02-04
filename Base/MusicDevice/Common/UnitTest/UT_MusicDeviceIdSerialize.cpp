#include <gtest/gtest.h>
#include "MusicDeviceId.h"

using namespace base::musicDevice;

TEST(MusicDeviceIdSerializationTest, serialize)
{
   MusicDeviceId id("hello", "bello");
   auto j = nlohmann::json(id);
   ASSERT_EQ(j.dump(), "{\"deviceName\":\"hello\",\"portName\":\"bello\"}");
}

TEST(MusicDeviceIdSerializationTest, deserialize)
{
   auto j = nlohmann::json::parse("{\"deviceName\":\"hello\",\"portName\":\"bello\"}");
   auto id = j.get<MusicDeviceId>();
}