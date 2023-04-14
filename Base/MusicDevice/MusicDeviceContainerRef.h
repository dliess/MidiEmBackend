#ifndef MUSIC_DEVICE_CONTAINER_REF_H
#define MUSIC_DEVICE_CONTAINER_REF_H

#include "MusicDeviceId.h"
#include "function_ref.h"

namespace base::musicDevice::sound { class SoundHandler; }

namespace base::musicDevice
{
class MusicDeviceContainerRef
{
public:
   template <class MusicDeviceContainer>
   requires (!std::is_same_v<std::decay_t<MusicDeviceContainer>, MusicDeviceContainerRef>)
   MusicDeviceContainerRef(MusicDeviceContainer& mdc) :
       m_pTypeErasedObj(std::addressof(mdc)),
       m_vtable({
           [](void* obj, const musicDevice::MusicDeviceId& mdId,
              util::function_ref<void(sound::SoundHandler&)> cb) {
              static_cast<MusicDeviceContainer*>(obj)->withSoundHandler(mdId,
                                                                        cb);
           }
       })
   {
   }
   void withSoundHandler(const musicDevice::MusicDeviceId& mdId,
                         util::function_ref<void(sound::SoundHandler&)> cb)
   {
      m_vtable.fn_withSoundHandler(m_pTypeErasedObj, mdId, cb);
   }

private:
   void* m_pTypeErasedObj{nullptr};
   struct VTable {
   void (*fn_withSoundHandler)(void* obj, const musicDevice::MusicDeviceId&,
                               util::function_ref<void(sound::SoundHandler&)>) =
       nullptr;
   };
   VTable m_vtable;
};

}   // namespace base::musicDevice

#endif