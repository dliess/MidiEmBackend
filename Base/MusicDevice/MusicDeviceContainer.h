#ifndef MUSIC_DEVICE_CONTAINER_H
#define MUSIC_DEVICE_CONTAINER_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "Identifiable.h"
#include "CallbackSignal.h"

namespace base::musicDevice
{
class MusicDeviceContainer
   : private std::unordered_map<util::Identifiable::UUID, std::shared_ptr<MusicDevice>>
//: public util::KeyValueVector<MusicDeviceId, std::shared_ptr<MusicDevice>>
{
public:
   using Super =
      std::unordered_map<util::Identifiable::UUID, std::shared_ptr<MusicDevice>>;
   inline MusicDeviceContainer();
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

   CB_SIGNAL(AboutToAdd, std::shared_ptr<MusicDevice>);
   CB_SIGNAL(Added, std::shared_ptr<MusicDevice>);
   CB_SIGNAL(AboutToRemove, std::shared_ptr<MusicDevice>);
   CB_SIGNAL(SoundDevParamChanged, util::Identifiable::UUID, int, int, float, float);
   CB_SIGNAL(ArpBypassChanged, util::Identifiable::UUID, int, bool);
   CB_SIGNAL(ArpRangeTypeChanged, util::Identifiable::UUID, int, base::arp::RangeType);
   CB_SIGNAL(ArpRangeChanged, util::Identifiable::UUID, int, int);
   CB_SIGNAL(ArpGateFillChanged, util::Identifiable::UUID, int, float);
   CB_SIGNAL(ArpStepLengthChanged, util::Identifiable::UUID, int, int);
   CB_SIGNAL(ArpAlgorithmChanged, util::Identifiable::UUID, int, base::arp::Algorithm);
   CB_SIGNAL(ArpHoldNotesChanged, util::Identifiable::UUID, int, bool);
   CB_SIGNAL(ArpFeedModeChanged, util::Identifiable::UUID, int, base::arp::FeedMode);
   CB_SIGNAL(ArpSeqSizeChanged, util::Identifiable::UUID, int, int);
   CB_SIGNAL(LFOWaveformChanged, util::Identifiable::UUID, int, int, sound::lfo::Waveform);
   CB_SIGNAL(LFOAmplitudeChanged, util::Identifiable::UUID, int, int, float);
   CB_SIGNAL(LFOFrequencyChanged, util::Identifiable::UUID, int, int, float);
   CB_SIGNAL(LFOMultiplierExpChanged, util::Identifiable::UUID, int, int, uint32_t);
   CB_SIGNAL(EnginePresetChanged, const std::string&, int, const std::string&);
   CB_SIGNAL(ActualPresetChanged, util::Identifiable::UUID, int, const std::string&);
};

} // namespace base::musicDevice

#include "MusicDeviceContainer.inl"
#include "MusicDeviceContainerMeta.h"

#endif