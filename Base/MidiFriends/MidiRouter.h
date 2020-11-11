#ifndef MIDI_ROUTER_H
#define MIDI_ROUTER_H

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Midi.h"
#include "MidiHolder.h"
#include "MidiMessage.h"
#include "MusicDevice.h"
#include "MusicDeviceId.h"
#include "Settings.h"

namespace base
{
namespace midifriends
{
struct RoutingDataSpecialized
{
   bool transmitClockMsg{true};
   using ChannelMap = std::array<uint16_t, midi::NUM_CHANNELS>;
   ChannelMap channelMapping;
};

struct RoutingData
{
   std::shared_ptr<musicDevice::MusicDevice::MidiOutput> pMidiOut;
   std::optional<RoutingDataSpecialized> specialized;
};

class Router : public utils::Settings<Router>
{
public:
   Router(musicDevice::MidiHolder& rMidiHolder) noexcept;
   bool isRoutedTo(const musicDevice::MidiHolder::Id& source,
                   const musicDevice::MidiHolder::Id& dest) const noexcept;
   void toggleRouted(const musicDevice::MidiHolder::Id& source,
                     const musicDevice::MidiHolder::Id& dest) noexcept;
   bool hasSpecializedData(
      const musicDevice::MidiHolder::Id& source,
      const musicDevice::MidiHolder::Id& dest) const noexcept;
   void initSpecialized(const musicDevice::MidiHolder::Id& source,
                        const musicDevice::MidiHolder::Id& dest) noexcept;
   void clearSpecialized(const musicDevice::MidiHolder::Id& source,
                         const musicDevice::MidiHolder::Id& dest) noexcept;
   uint16_t getMappingFor(const musicDevice::MidiHolder::Id& source,
                          const musicDevice::MidiHolder::Id& dest,
                          int sourceChannelIdx) const noexcept;
   void setMappingForChannelIdx(const musicDevice::MidiHolder::Id& source,
                                const musicDevice::MidiHolder::Id& dest,
                                int sourceChannelIdx, int destinationChannelIdx,
                                bool enable) noexcept;
   void registerChangedCb(std::function<void(void)> cb) noexcept;
   void registerSpecChangedCb(std::function<void(void)> cb) noexcept;

   using DstType = std::unordered_map<musicDevice::MidiHolder::Id, RoutingData>;
   using RoutingDataMap = std::unordered_map<musicDevice::MidiHolder::Id, DstType>;
   // ============== Settings ===============
   using Settings = RoutingDataMap;
   Settings getSettings() const noexcept;
   void setSettings(const Settings& settings) noexcept;
   // =======================================
private:
   musicDevice::MidiHolder& m_rMidiHolder;
   RoutingDataMap m_routingData;
   std::vector<std::function<void(void)>> m_cb;
   std::vector<std::function<void(void)>> m_cbSpecSchanged;

   void handleMidiIn(const musicDevice::MidiHolder::Id& id,
                     const midi::MidiMessage& midiMsg) noexcept;
   void handleSpecialized(
      const midi::MidiMessage& midiMsg,
      const RoutingDataSpecialized& routingData,
      musicDevice::MusicDevice::MidiOutput& midiOut) noexcept;

   template<typename Msg>
   void handleVoiceMsg(const RoutingDataSpecialized::ChannelMap& channelMap,
                       const Msg& msg,
                       musicDevice::MusicDevice::MidiOutput& midiOut) noexcept;

   inline const RoutingData* getRoutingData(
      const musicDevice::MidiHolder::Id& source,
      const musicDevice::MidiHolder::Id& dest) const noexcept;
   inline RoutingData* getRoutingData(
      const musicDevice::MidiHolder::Id& source,
      const musicDevice::MidiHolder::Id& dest) noexcept;
};

} // namespace midifriends
} // namespace base

#include "MidiRouter.inl"
#include "MidiRouterMeta.h"

#endif