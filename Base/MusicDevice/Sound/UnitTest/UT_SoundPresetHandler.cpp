#include <gtest/gtest.h>
#include "SoundPresetHandler.h"
#include "SoundSection.h"
#include "SoundParameterStorage.h"

using namespace base::musicDevice;

TEST(SoundPresetHandlerTest, test)
{
   description::sound::Section soundSection;
   sound::ParameterStorage soundParameterStorage(soundSection);
   sound::preset::PresetHandler soundPresetHandler(soundSection, soundParameterStorage, nullptr);
}