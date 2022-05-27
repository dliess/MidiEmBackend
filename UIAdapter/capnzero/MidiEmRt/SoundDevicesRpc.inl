#ifndef SOUND_DEVICES_RPC_INL
#define SOUND_DEVICES_RPC_INL

#include "Identifiable.h"
#include "MusicDeviceContainer.h"

namespace uiadapter::capnzero
{
template <typename Cb>
void SoundDevicesRpc::withSoundHandlerDo(const ::capnzero::SpanCL<16>& uuid,
                                         Cb&& cb)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      if (iter->second->soundHandler)
      {
         try
         {
            cb(iter->second->soundHandler.value());
         }
         catch (const std::exception& e)
         {
            spdlog::error("Exception caught: {}", e.what());
         }
      }
      else
      {
         spdlog::error("{} is not a sound-device",
                       iter->second->deviceId().deviceName);
      }
   }
   else
   {
      spdlog::error("No music-device found with uuid {}",
                    util::uuid2Str(uuid_));
   }
}

}   // namespace uiadapter::capnzero

#endif