#ifndef KIT_INSTRUMENT_H
#define KIT_INSTRUMENT_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "Instrument.h"
#include "MusicDeviceId.h"

namespace base
{
namespace musicDevice
{
struct MusicDevice;
struct Holder;
} // namespace musicDevice

namespace instruments
{
struct VoiceDescr : public util::Identifiable
{
   musicDevice::MusicDeviceId soundDeviceId;
   std::shared_ptr<musicDevice::MusicDevice> pSoundDevice;
   int voiceIndex;
   int noteOffset;
   void updateMusicDevicePtr(musicDevice::Holder& rMusicDeviceHolder) noexcept;
};
struct KitSound : public util::Identifiable
{
   inline KitSound() noexcept = default;
   inline KitSound(std::string name) noexcept;
   std::string name;
//   static constexpr int NUM_MAX_VOICES_PER_KIT_VOICE = 4;
//   using Voices = std::array<VoiceDescr, NUM_MAX_VOICES_PER_KIT_VOICE>;
   using Voices = std::vector<VoiceDescr>;
   Voices voices;
};

class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   KitInstrument(std::string name) noexcept;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
   template<typename T>
   void addSound(T&& kitSound) noexcept;

   inline std::string name() const noexcept;
   inline void setName(const std::string& name) noexcept;
   inline std::vector<KitSound>& sounds() noexcept;
   inline const std::vector<KitSound>& sounds() const noexcept;

   template<typename Cb>
   void forEachVoice(Cb&& cb);

   friend auto meta::registerMembers<KitInstrument>();

private:
   std::string m_name;
   std::vector<KitSound> m_sounds;
};

} // namespace instruments
} // namespace base

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif // KIT_INSTRUMENT_H