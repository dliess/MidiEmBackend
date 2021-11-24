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
         const std::string musicDeviceName = ptr->deviceId().deviceName;
         ptr->soundHandler->registerLFOWaveformChangeCB([this, uuid](int voiceId, int paramId, sound::lfo::Waveform waveform){
            for(auto& cb : m_lFOWaveformChangeCBs) cb(uuid, voiceId, paramId, waveform);
         });
         ptr->soundHandler->registerLFOAmplitudeChangeCB([this, uuid](int voiceId, int paramId, float amplitude){
            for(auto& cb : m_lFOAmplitudeChangeCB) cb(uuid, voiceId, paramId, amplitude);
         });
         ptr->soundHandler->registerLFOFrequencyChangeCB([this, uuid](int voiceId, int paramId, float frequency){
            for(auto& cb : m_lFOFrequencyChangeCB) cb(uuid, voiceId, paramId, frequency);
         });
         ptr->soundHandler->registerLFOMultiplierExpChangeCB([this, uuid](int voiceId, int paramId, uint32_t multiplierExp){
            for(auto& cb : m_lFOMultiplierExpChangeCB) cb(uuid, voiceId, paramId, multiplierExp);
         });
         ptr->soundHandler->presetHandler().registerChangedCb([this, musicDeviceName](int engineIdx, const std::string& presetName){
            for(auto& cb : m_enginePresetChangeCB) cb(musicDeviceName, engineIdx, presetName);
         });
         assert(ptr->soundHandler->arpeggiators().size());
         for(int voiceIdx = 0; voiceIdx < ptr->soundHandler->arpeggiators().size(); ++voiceIdx)
         {
            ptr->soundHandler->arpeggiators().at(voiceIdx).onBypassChanged([this, uuid, voiceIdx](bool on){
               emitArpBypassChanged(uuid, voiceIdx, on);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onRangeTypeChanged([this, uuid, voiceIdx](base::arp::RangeType type){
               emitArpRangeTypeChanged(uuid, voiceIdx, type);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onRangeChanged([this, uuid, voiceIdx](int range){
               emitArpRangeChanged(uuid, voiceIdx, range);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onGateFillChanged([this, uuid, voiceIdx](float value){
               emitArpGateFillChanged(uuid, voiceIdx, value);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onStepLengthChanged([this, uuid, voiceIdx](int value){
               emitArpStepLengthChanged(uuid, voiceIdx, value);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onAlgorithmChanged([this, uuid, voiceIdx](base::arp::Algorithm algorithm){
               emitArpAlgorithmChanged(uuid, voiceIdx, algorithm);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onHoldNotesChanged([this, uuid, voiceIdx](bool on){
               emitArpHoldNotesChanged(uuid, voiceIdx, on);
            });
         }
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

inline
void MusicDeviceContainer::registerLFOWaveformChangeCB(LFOWaveformChangeCB cb)
{
   m_lFOWaveformChangeCBs.push_back(cb);
}

inline
void MusicDeviceContainer::registerLFOAmplitudeChangeCB(LFOAmplitudeChangeCB cb)
{
   m_lFOAmplitudeChangeCB.push_back(cb);
}

inline
void MusicDeviceContainer::registerLFOFrequencyChangeCB(LFOFrequencyChangeCB cb)
{
   m_lFOFrequencyChangeCB.push_back(cb);
}

inline 
void MusicDeviceContainer::registerLFOMultiplierExpChangeCB(LFOMultiplierExpChangeCB cb)
{
   m_lFOMultiplierExpChangeCB.push_back(cb);
}

inline
void MusicDeviceContainer::registerEnginePresetChangeCB(EnginePresetChangeCB cb)
{
   m_enginePresetChangeCB.push_back(cb);
}

} // namespace base::musicDevice

#endif