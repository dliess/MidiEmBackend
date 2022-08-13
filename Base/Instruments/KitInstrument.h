#ifndef KIT_INSTRUMENT_H
#define KIT_INSTRUMENT_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "Instrument.h"
#include "MusicDeviceId.h"
#include "CompositeSound.h"

namespace base
{
namespace musicDevice
{
struct MusicDevice;
struct Holder;
} // namespace musicDevice

namespace instruments
{

class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   KitInstrument(std::string name) noexcept;
   void noteOn(int soundIndex, int note, float velocity) noexcept;
   void noteOff(int soundIndex, int note, float velocity) noexcept;
   template<typename T>
   void addSound(T&& kompositeSound) noexcept;

   inline std::string name() const noexcept;
   inline void setName(const std::string& name) noexcept;
   inline std::vector<CompositeSound>& sounds() noexcept;
   inline const std::vector<CompositeSound>& sounds() const noexcept;

   template<typename Cb>
   void forEachVoice(Cb&& cb);

   friend auto meta::registerMembers<KitInstrument>();

private:
   std::string m_name;
   std::vector<CompositeSound> m_compositeSounds;
};

} // namespace instruments
} // namespace base

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif // KIT_INSTRUMENT_H