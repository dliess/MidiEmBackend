#ifndef MUSIC_DEVICE_CONTAINER_INL
#define MUSIC_DEVICE_CONTAINER_INL

#include <loguru.hpp>

#include "MusicDeviceContainer.h"
#include "SoundParameterStorage.h"

namespace base::musicDevice
{

inline MusicDeviceContainer::MusicDeviceContainer() : Super()
{
   registerForAdd([this](const std::shared_ptr<MusicDevice>& ptr){
      if(ptr->soundHandler)
      {
         const auto uuid = ptr->id();
         ptr->soundHandler->registerLFOWaveformChangeCB([this, &uuid](int voiceId, int paramId, sound::LFO::Waveform waveform){
            for(auto& cb : m_lFOWaveformChangeCBs) cb(uuid, voiceId, paramId, waveform);
         });
         ptr->soundHandler->registerLFOAmplitudeChangeCB([this, &uuid](int voiceId, int paramId, float amplitude){
            for(auto& cb : m_lFOAmplitudeChangeCB) cb(uuid, voiceId, paramId, amplitude);
         });
         ptr->soundHandler->registerLFOFrequencyChangeCB([this, &uuid](int voiceId, int paramId, float frequency){
            for(auto& cb : m_lFOFrequencyChangeCB) cb(uuid, voiceId, paramId, frequency);
         });
      }
   });
}

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
         e.second->soundHandler->forEachParameter([this, &e](int voiceIdx, int paramIdx, sound::ParameterStorage::Element& element) {
            const auto changedValues = element.uiAsksForChangedValues();
            if(changedValues)
            {
               for(auto &cb : m_paramChangeCbsUI)
               {
                  cb(e.second->id(), voiceIdx, paramIdx, changedValues->first, changedValues->second);
               }
            }
         });
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

inline void MusicDeviceContainer::registerSoundDevParamChangeCbUI(SoundDevParamChangeCb cb)
{
   m_paramChangeCbsUI.push_back(cb);
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

inline MusicDeviceContainer::const_iterator MusicDeviceContainer::findByDeviceId(const MusicDeviceId& mdId) const noexcept
{
   for(auto iter = Super::begin(); iter != Super::end(); ++iter)
   {
      if(iter->second.get()->deviceId() == mdId)
      {
         return iter;
      }
   }
   return Super::end();
}

inline MusicDeviceContainer::iterator MusicDeviceContainer::findByDeviceId(const MusicDeviceId& mdId) noexcept
{
   for(auto iter = Super::begin(); iter != Super::end(); ++iter)
   {
      if(iter->second.get()->deviceId() == mdId)
      {
         return iter;
      }
   }
   return Super::end();
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

inline MusicDeviceContainer::size_type MusicDeviceContainer::eraseByDeviceId(const MusicDeviceId& mdId)
{
   auto it = findByDeviceId(mdId);
   if(it != end())
   {
      invokeAboutToRemoveCbs(it->second);
      return Super::erase(it->first);
   }
   return 0; // Number of elements removed
}

inline size_t MusicDeviceContainer::size() const noexcept
{
   return Super::size();
}

void MusicDeviceContainer::registerLFOWaveformChangeCB(LFOWaveformChangeCB cb)
{
   m_lFOWaveformChangeCBs.push_back(cb);
}

void MusicDeviceContainer::registerLFOAmplitudeChangeCB(LFOAmplitudeChangeCB cb)
{
   m_lFOAmplitudeChangeCB.push_back(cb);
}

void MusicDeviceContainer::registerLFOFrequencyChangeCB(LFOFrequencyChangeCB cb)
{
   m_lFOFrequencyChangeCB.push_back(cb);
}


} // namespace base::musicDevice

#endif