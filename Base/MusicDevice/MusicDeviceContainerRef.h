#ifndef MUSIC_DEVICE_CONTAINER_REF_H
#define MUSIC_DEVICE_CONTAINER_REF_H

#include "MusicDeviceId.h"
#include "function_ref.h"
#include "ErrorHandling.h"

namespace base::musicDevice::sound { class SoundHandler; }

namespace base::musicDevice
{

namespace detail { 
class MusicDeviceContainerRefConcept
{
public:
   virtual ~MusicDeviceContainerRefConcept() = default;
   virtual void withSoundHandler(const musicDevice::MusicDeviceId& mdId,
                                 util::function_ref<void(sound::SoundHandler&)> cb) = 0;
   virtual Ret<sound::SoundHandler*> getSoundHandler(const musicDevice::MusicDeviceId& mdId) = 0;
};
template <class MusicDeviceContainerRefImpl>
class MusicDeviceContainerRefAdaptor : public MusicDeviceContainerRefConcept
{
public:
   MusicDeviceContainerRefAdaptor(MusicDeviceContainerRefImpl* pTypeErasedObj) noexcept : m_obj(pTypeErasedObj) {}
   void withSoundHandler(const musicDevice::MusicDeviceId& mdId,
                         util::function_ref<void(sound::SoundHandler&)> cb) override
   {
      m_obj->withSoundHandler(mdId, cb);
   }
   Ret<sound::SoundHandler*> getSoundHandler(const musicDevice::MusicDeviceId& mdId) override
   {
      return m_obj->getSoundHandler(mdId);
   }
private:
   MusicDeviceContainerRefImpl* m_obj{nullptr};
};

}  // namespace detail

class MusicDeviceContainerRef
{
public:
   template <class T>
   requires (!std::is_same_v<std::decay_t<T>, MusicDeviceContainerRef>)
   MusicDeviceContainerRef(T& mdContainer) noexcept 
   {
      static_assert(sizeof(detail::MusicDeviceContainerRefAdaptor<T>) == sizeof(implBuf));
      new (&implBuf) detail::MusicDeviceContainerRefAdaptor<T>{std::addressof(mdContainer)};
   }
   void withSoundHandler(const musicDevice::MusicDeviceId& mdId,
                         util::function_ref<void(sound::SoundHandler&)> cb)
   { 
      getImpl().withSoundHandler(mdId, cb); 
   }
   Ret<sound::SoundHandler*> getSoundHandler(const musicDevice::MusicDeviceId& mdId)
   {
      return getImpl().getSoundHandler(mdId);
   }

private:
   alignas(2 * sizeof(void*))
      std::array<std::byte, 2 * sizeof(void*) > implBuf;
   detail::MusicDeviceContainerRefConcept& getImpl() noexcept
   {
      return *reinterpret_cast<detail::MusicDeviceContainerRefConcept*>(implBuf.data());
   }
};

}   // namespace base::musicDevice

#endif
