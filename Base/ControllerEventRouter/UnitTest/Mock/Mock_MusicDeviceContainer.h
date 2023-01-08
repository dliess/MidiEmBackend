#ifndef MOCK_MUSICDEVICECONTAINER_H
#define MOCK_MUSICDEVICECONTAINER_H

#include "gmock/gmock.h"
#include "Identifiable.h"
#include "function_ref.h"
#include "SoundHandler.h"

namespace base::musicDevice::mock
{
class MockMusicDeviceContainer
{
public:
   MOCK_METHOD(void, withSoundHandler, (util::Identifiable::UUIDView, util::function_ref<void(sound::SoundHandler&)>));
};

}   // namespace mock

#endif