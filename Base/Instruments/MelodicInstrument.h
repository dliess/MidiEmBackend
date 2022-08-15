#ifndef MELODIC_INSTRUMENT_H
#define MELODIC_INSTRUMENT_H

#include <array>
#include <memory>
#include <vector>

#include "CompositeSound.h"
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
   std::shared_ptr<musicDevice::MusicDevice> pSoundDevice;
   musicDevice::MusicDeviceId soundDeviceId;
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
   void setParameterValue(int compPart, int parameterId, float value) noexcept;
   void incrementParameterValue(int compPart, int parameterId, float increment) noexcept;

   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   using VoiceContainer = std::vector<CompositeSound>;
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