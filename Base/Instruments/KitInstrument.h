#ifndef KIT_INSTRUMENT_H
#define KIT_INSTRUMENT_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "CompositeSound.h"
#include "Instrument.h"
#include "MusicDeviceId.h"

namespace base
{
namespace musicDevice
{
struct MusicDevice;
struct Holder;
}   // namespace musicDevice

namespace instruments
{
class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   KitInstrument(std::string name) noexcept;
   KitInstrument(const KitInstrument& rhs) noexcept = delete;
   KitInstrument& operator=(const KitInstrument& rhs) noexcept = delete;
   KitInstrument(KitInstrument&& rhs) noexcept                 = default;
   KitInstrument& operator=(KitInstrument&& rhs) noexcept = default;
   void noteOn(int note, float velocity) noexcept override;
   void noteOff(int note, float velocity) noexcept override;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
   void incrementParameterValue(int soundIdx, int componentIdx,
                                int parameterIdx, float increment,
                                bool roundRobin = false) noexcept;
   [[nodiscard]] float getParameterValue(
       int soundIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterPart parameterPart =
           musicDevice::sound::ParameterPart::Commanded) const noexcept;
   void setParameterValue(int soundIdx, int componentIdx, int parameterIdx,
                          float value) noexcept;
   template <typename T> void addSound(int padIdx, T&& kompositeSound) noexcept;

   inline std::string name() const noexcept;
   inline void setName(const std::string& name) noexcept;
   inline std::vector<CompositeSound>& sounds() noexcept;
   inline const std::vector<CompositeSound>& sounds() const noexcept;

   template <typename Cb> void forEachVoice(Cb&& cb);

   friend auto meta::registerMembers<KitInstrument>();

private:
   std::string m_name;
   std::vector<CompositeSound> m_compositeSounds;
   std::optional<int> toSoundIndex(int note) const noexcept;
};

}   // namespace instruments
}   // namespace base

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H