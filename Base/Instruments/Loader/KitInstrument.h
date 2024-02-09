#ifndef KIT_INSTRUMENT_LOADER_H
#define KIT_INSTRUMENT_LOADER_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "Instrument.h"
#include "KitInstrumentVoice.h"
#include "MusicDeviceId.h"
#include "function_ref.h"

namespace base
{
namespace musicDevice
{
struct MusicDevice;
struct Holder;
}   // namespace musicDevice

namespace instruments::loader
{

class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   explicit KitInstrument(std::string name) noexcept;

   [[nodiscard]] std::optional<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) const;

   void clearModifier(int voiceIdx, int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr) const;
   void applyModifier(int voiceIdx, int componentIdx, std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity) const;


   [[nodiscard]] const musicDevice::description::sound::Parameter*
   parameterDescription(int voiceIdx, int componentIdx, int parameterIdx) const;

   template <typename T> void addVoice(int padIdx, T&& voice);


   std::string name() const noexcept;
   void setName(const std::string& name) noexcept;

   std::vector<KitVoice>& voices() noexcept;
   const std::vector<KitVoice>& voices() const noexcept;

   template <typename Cb> void forEachComponent(Cb&& cb);
   template <typename Cb> void forEachComponent(Cb&& cb) const;
   template <typename Cb> void forEachComponentExt(Cb&& cb);
   template <typename Cb> void forEachComponentExt(Cb&& cb) const;

   friend void to_json(nlohmann::json& j, const KitInstrument& instr);
   friend void from_json(const nlohmann::json& j, KitInstrument& instr);
   friend bool isSameInstrument(const KitInstrument& lhs,
                                const KitInstrument& rhs);

   void setVoiceNoteOffset(int voiceIdx, int offset);
   void setComponentNoteOffset(int voiceIdx, int componentIdx, int offset);

   void setVoiceAmp(int voiceIdx, float amp);
   void setComponentAmp(int voiceIdx, int componentIdx, float amp);

   CB_SIGNAL_SINGLE_SUBSCRIBER(ComponentNoteOffsetChanged,int, int, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(VoiceNoteOffsetChanged, int, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ComponentAmpChanged,int, int, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(VoiceAmpChanged, int, float);

private:
   std::string m_name;
   std::vector<KitVoice> m_voices;
   std::optional<int> toVoiceIndex(int note) const;
   inline void withComponent(
       int voiceIdx, int componentIdx,
       util::function_ref<void(const Component&)> cb) const;
};

}   // namespace instruments::loader
}   // namespace base

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H
