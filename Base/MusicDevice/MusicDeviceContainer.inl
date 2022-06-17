#ifndef MUSIC_DEVICE_CONTAINER_INL
#define MUSIC_DEVICE_CONTAINER_INL

#include <spdlog/spdlog.h>

#include "MusicDeviceContainer.h"
#include "SoundParameterStorage.h"

namespace base::musicDevice
{

inline MusicDeviceContainer::MusicDeviceContainer() : Super()
{
   onAdded([this](const std::shared_ptr<MusicDevice>& ptr){
      if(ptr->soundHandler)
      {
         const auto uuid = ptr->id();
         const std::string musicDeviceName = ptr->deviceId().deviceName;
         ptr->soundHandler->onSoundDevParamChanged([this, uuid](int voiceId, int paramId, float commanded, float actual){
            emitSoundDevParamChanged(uuid, voiceId, paramId, commanded, actual);
         });
         ptr->soundHandler->onLFOWaveformChanged([this, uuid](int voiceId, int paramId, sound::lfo::Waveform waveform){
            emitLFOWaveformChanged(uuid, voiceId, paramId, waveform);
         });
         ptr->soundHandler->onLFOAmplitudeChanged([this, uuid](int voiceId, int paramId, float amplitude){
            emitLFOAmplitudeChanged(uuid, voiceId, paramId, amplitude);
         });
         ptr->soundHandler->onLFOFrequencyChanged([this, uuid](int voiceId, int paramId, float frequency){
            emitLFOFrequencyChanged(uuid, voiceId, paramId, frequency);
         });
         ptr->soundHandler->onLFOMultiplierExpChanged([this, uuid](int voiceId, int paramId, uint32_t multiplierExp){
            emitLFOMultiplierExpChanged(uuid, voiceId, paramId, multiplierExp);
         });
         ptr->soundHandler->presetHandler().onPresetChanged([this, musicDeviceName](int engineIdx, const std::string& presetName){
            emitEnginePresetChanged(musicDeviceName, engineIdx, presetName);
         });
         ptr->soundHandler->onActualPresetChanged([this, uuid](int voiceIdx, const std::string& presetName){
            emitActualPresetChanged(uuid, voiceIdx, presetName);
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
            ptr->soundHandler->arpeggiators().at(voiceIdx).onStepLengthChanged([this, uuid, voiceIdx](float value){
               emitArpStepLengthChanged(uuid, voiceIdx, value);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onAlgorithmChanged([this, uuid, voiceIdx](base::arp::Algorithm algorithm){
               emitArpAlgorithmChanged(uuid, voiceIdx, algorithm);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onHoldNotesChanged([this, uuid, voiceIdx](bool on){
               emitArpHoldNotesChanged(uuid, voiceIdx, on);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onFeedModeChanged([this, uuid, voiceIdx](base::arp::FeedMode feedMode){
               emitArpFeedModeChanged(uuid, voiceIdx, feedMode);
            });
            ptr->soundHandler->arpeggiators().at(voiceIdx).onSeqSizeChanged([this, uuid, voiceIdx](int seqSize){
               emitArpSeqSizeChanged(uuid, voiceIdx, seqSize);
            });
         }
      }
      if(ptr->controllerHandler)
      {
         const auto uuid = ptr->id();
         ptr->controllerHandler->onEventReceived([this, uuid](const controller::Event& event) {
            emitControllerDevEventOccured(uuid, event);
         });
         ptr->controllerHandler->onEventReceivedUI([this, uuid](const controller::Event& event) {
            emitControllerDevEventOccuredUI(uuid, event);
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

inline void MusicDeviceContainer::updateMDParameterUI()
{
   for (auto& e : *this)
   {
      assert(e.second);
      if (e.second->soundHandler)
      {
         e.second->soundHandler->triggerUICallbacks();
      }
      if (e.second->controllerHandler)
      {
         e.second->controllerHandler->triggerUICallbacks();
      }
   }
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
   emitAboutToAdd(*val.second.get());
   const auto ret = Super::insert(val);
   emitAdded(val.second);
   return ret;
}

inline MusicDeviceContainer::size_type MusicDeviceContainer::erase(
   const key_type& k)
{
   auto it = find(k);
   if(it != end())
   {
      emitAboutToRemove(it->second);
   }
   return Super::erase(k);
}

inline MusicDeviceContainer::size_type MusicDeviceContainer::eraseByDeviceId(const MusicDeviceId& mdId)
{
   auto it = findByDeviceId(mdId);
   if(it != end())
   {
      emitAboutToRemove(it->second);
      return Super::erase(it->first);
   }
   return 0; // Number of elements removed
}

inline size_t MusicDeviceContainer::size() const noexcept
{
   return Super::size();
}

} // namespace base::musicDevice

#endif