#include <gtest/gtest.h>
#include "MusicDevice.h"
#include "MusicDeviceDescription.h"
#include "SoundPresets.h"

using namespace base::musicDevice;

TEST(MusicDeviceTest, test)
{
   MusicDeviceId id;
   MusicDevice musicDevice(id,
                           "resource/root/dir", 
                           std::make_shared<description::Description>(),
                           std::make_shared<sound::SoundPresets>("manufacturer", "product"));

   MusicDevice other(std::move(musicDevice));
}