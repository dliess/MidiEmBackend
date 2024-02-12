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

namespace base::instruments::loader
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
                          float value);

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

   friend auto meta::registerMembers<KitInstrument>();
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
       util::function_ref<void(KitComponent&)> cb);
   inline void withConstComponent(
       int voiceIdx, int componentIdx,
       util::function_ref<void(const KitComponent&)> cb) const;
};

}   // namespace base::instruments::loader

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H
