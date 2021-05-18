#ifndef MUSIC_DEVICE_CONTAINER_H
#define MUSIC_DEVICE_CONTAINER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "Identifiable.h"

namespace base::musicDevice
{
class MusicDeviceContainer
   : private std::unordered_map<util::Identifiable::UUID, std::shared_ptr<MusicDevice>>
//: public util::KeyValueVector<MusicDeviceId, std::shared_ptr<MusicDevice>>
{
public:
   using Super =
      std::unordered_map<util::Identifiable::UUID, std::shared_ptr<MusicDevice>>;
   using Cb = std::function<void(std::shared_ptr<MusicDevice>)>;
   inline void registerForAdd(Cb cb) noexcept;
   inline void registerForAboutToRemove(Cb cb) noexcept;
   inline void updateSoundParameterActualValues();
   inline void updateSoundParameterUI();

   using iterator       = Super::iterator;
   using const_iterator = Super::const_iterator;
   using key_type       = Super::key_type;
   using mapped_type    = Super::mapped_type;
   using value_type     = Super::value_type;
   using size_type      = Super::size_type;

   inline iterator begin() noexcept;
   inline const_iterator begin() const noexcept;
   inline iterator end() noexcept;
   inline const_iterator end() const noexcept;
   inline iterator find(const key_type& key) noexcept;
   inline const_iterator findByDeviceId(const MusicDeviceId& mdId) const noexcept;
   inline iterator findByDeviceId(const MusicDeviceId& mdId) noexcept;
   inline const_iterator find(const key_type& key) const noexcept;
   inline std::pair<iterator, bool> insert(const value_type& val);
   inline size_type erase(const key_type& k);
   inline size_type eraseByDeviceId(const MusicDeviceId& mdId);
   
   inline size_t size() const noexcept;

private:
   std::vector<Cb> m_addedCb;
   std::vector<Cb> m_aboutToRemoveCbs;
   inline void invokeAddCbs(const std::shared_ptr<MusicDevice>& ptr);
   inline void invokeAboutToRemoveCbs(const std::shared_ptr<MusicDevice>& ptr);
};

} // namespace base::musicDevice

#include "MusicDeviceContainer.inl"

#endif