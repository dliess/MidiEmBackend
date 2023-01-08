#ifndef MUSIC_DEVICE_CONTAINER_REF_H
#define MUSIC_DEVICE_CONTAINER_REF_H

#include "Identifiable.h"
#include "function_ref.h"

namespace base::musicDevice::sound { class SoundHandler; }

namespace base::musicDevice
{
class MusicDeviceContainerRef
{
public:
   template <class MusicDeviceContainer>
   MusicDeviceContainerRef(MusicDeviceContainer& mdc) :
       m_pTypeErasedObj(std::addressof(mdc)),
       m_vtable({
           [](void* obj, util::Identifiable::UUIDView uuid,
              util::function_ref<void(sound::SoundHandler&)> cb) {
              static_cast<MusicDeviceContainer*>(obj)->withSoundHandler(uuid,
                                                                        cb);
           }
       })
   {
   }
   void withSoundHandler(util::Identifiable::UUIDView uuid,
                         util::function_ref<void(sound::SoundHandler&)> cb)
   {
      m_vtable.fn_withSoundHandler(m_pTypeErasedObj, uuid, cb);
   }

private:
   void* m_pTypeErasedObj{nullptr};
   struct VTable {
   void (*fn_withSoundHandler)(void* obj, util::Identifiable::UUIDView,
                               util::function_ref<void(sound::SoundHandler&)>) =
       nullptr;
   };
   VTable m_vtable;
};

}   // namespace base::musicDevice

#endif