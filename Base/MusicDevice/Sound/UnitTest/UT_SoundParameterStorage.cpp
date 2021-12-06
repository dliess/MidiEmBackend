#include <gtest/gtest.h>

#include "SoundParameterStorage.h"

using namespace base::musicDevice;

TEST(SoundParameterStorageTest, test)
{
   description::sound::Section soundSection;
   sound::ParameterStorage soundParameterStotrage(soundSection);
}