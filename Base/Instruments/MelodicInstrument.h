#ifndef MELODIC_INSTRUMENT_H
#define MELODIC_INSTRUMENT_H

#include <array>
#include <memory>
#include <vector>

#include "Instrument.h"
#include "MusicDeviceId.h"

class MusicDevice;

namespace base
{
namespace musicDevice
{
struct Holder;
struct MusicDevice;
}
namespace instruments
{
struct MelodicInstrumentVoice : public util::Identifiable
{
   musicDevice::MusicDeviceId soundDeviceId;
   std::shared_ptr<musicDevice::MusicDevice> pSoundDevice;
   int voiceIndex;
};

class MelodicInstrument : public Instrument
{
public:
   MelodicInstrument() = default;
   MelodicInstrument(std::string name) noexcept;
   void noteOn(int note, float velocity) noexcept;
   void noteOff(int note, float velocity) noexcept;
   void pitchBend(float value) noexcept;
   void parameterChange(int parameterId, float value) noexcept;

   void noteOn(int voiceIdx, int note, float velocity) noexcept;
   void noteOff(int voiceIdx, int note, float velocity) noexcept;
   void pitchBend(int voiceIdx, float value) noexcept;
   void parameterChange(int voiceIdx, int parameterId, float value) noexcept;

   template<typename T>
   void addVoice(T&& voice) noexcept;

   template <typename Cb>
   void forEachVoice(Cb&& cb);

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<MelodicInstrumentVoice>;
   VoiceContainer& voices() noexcept;
   friend auto meta::registerMembers<MelodicInstrument>();

private:
   VoiceContainer m_voices;
   std::string m_name;
   int m_currentVoiceIndex{-1};

   static constexpr int NUM_NOTES = 128;
   static constexpr int FREE      = -1; 
    
   std::array<int, NUM_NOTES> m_noteAllocations;

   inline void incrementVoiceIndex() noexcept;
};

} // namespace instruments
} // namespace base

#include "MelodicInstrument.inl"
#include "MelodicInstrumentMeta.h"

#endif // MELODIC_INSTRUMENT_H