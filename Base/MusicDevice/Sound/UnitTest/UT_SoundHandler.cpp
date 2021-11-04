#include <gtest/gtest.h>
#include "SoundHandler.h"
#include "SoundSection.h"
#include "DevicePresets.h"

using namespace base::musicDevice;

TEST(SoundHandlerTest, test)
{
   description::sound::Section soundSection;
   sound::SoundHandler soundHandler("TestDevice", soundSection, nullptr);
   soundHandler.noteOn(0, 1, 2);
   soundHandler.noteOff(0, 1, 2);
   soundHandler.pitchBend(0, 2);

   sound::SoundHandler other(std::move(soundHandler));
}