#ifndef MUSIC_DEVICE_CONTAINER_INL
#define MUSIC_DEVICE_CONTAINER_INL

#include <loguru.hpp>

#include "MusicDeviceContainer.h"

namespace base::musicDevice
{
inline void MusicDeviceContainer::updateSoundParameterActualValues()
{
   for (auto& e : *this)
   {
      assert(e.second);
      if (e.second->soundHandler)
      {
         e.second->soundHandler->updateActualSoundStorageValues();
      }
   }
}

inline void MusicDeviceContainer::updateSoundParameterUI()
{
   for (auto& e : *this)
   {
      assert(e.second);
      if (e.second->soundHandler)
      {
         e.second->soundHandler->updateUI();
      }
   }
}

inline void MusicDeviceContainer::registerForAdd(Cb cb) noexcept
{
   m_addedCb.push_back(cb);
   for(auto &val : *this)
   {
      cb(val.second);
   }
}

inline void MusicDeviceContainer::registerForAboutToRemove(Cb cb) noexcept
{
   m_aboutToRemoveCbs.push_back(cb);
}

inline void MusicDeviceContainer::invokeAddCbs(const std::shared_ptr<MusicDevice>& ptr)
{
   for (auto& cb : m_addedCb) cb(ptr);
}

inline void MusicDeviceContainer::invokeAboutToRemoveCbs(const std::shared_ptr<MusicDevice>& ptr)
{
   for (auto& cb : m_aboutToRemoveCbs) cb(ptr);
}

inline MusicDeviceContainer::iterator MusicDeviceContainer::begin() noexcept
{
   return Super::begin();
}

inline MusicDeviceContainer::const_iterator MusicDeviceContainer::begin()
   const noexcept
{
   return Super::begin();
}

inline MusicDeviceContainer::iterator MusicDeviceContainer::end() noexcept
{
   return Super::end();
}

inline MusicDeviceContainer::const_iterator MusicDeviceContainer::end()
   const noexcept
{
   return Super::end();
}

inline MusicDeviceContainer::iterator MusicDeviceContainer::find(
   const MusicDeviceContainer::key_type& key) noexcept
{
   return Super::find(key);
}

inline MusicDeviceContainer::const_iterator MusicDeviceContainer::find(
   const MusicDeviceContainer::key_type& key) const noexcept
{
   return Super::find(key);
}

inline std::pair<MusicDeviceContainer::iterator, bool>
MusicDeviceContainer::insert(const value_type& val)
{
   const auto ret = Super::insert(val);
   invokeAddCbs(val.second);
   return ret;
}

inline MusicDeviceContainer::size_type MusicDeviceContainer::erase(
   const key_type& k)
{
   auto it = find(k);
   if(it != end())
   {
      invokeAboutToRemoveCbs(it->second);
   }
   return Super::erase(k);
}

inline size_t MusicDeviceContainer::size() const noexcept
{
   return Super::size();
}

} // namespace base::musicDevice

#endif