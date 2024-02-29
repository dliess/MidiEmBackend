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
#include "ErrorHandling.h"

namespace base::instruments { class KitInstrumentCopyer; }

namespace base::instruments::loader
{

class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   explicit KitInstrument(std::string name) noexcept;

   [[nodiscard]] Ret<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   Void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value);

   [[nodiscard]] Ret<const musicDevice::description::sound::Parameter*>
   parameterDescription(int voiceIdx, int componentIdx, int parameterIdx) const;

   friend class KitInstrumentModifier;
   friend class KitInstrumentsModifier;
   friend auto meta::registerMembers<KitInstrument>();
   friend bool isSameInstrument(const KitInstrument& lhs,
                                const KitInstrument& rhs);

   Void setVoiceNoteOffset(int voiceIdx, int offset);
   Void setComponentNoteOffset(int voiceIdx, int componentIdx, int offset);

   Void setVoiceAmp(int voiceIdx, float amp);
   Void setComponentAmp(int voiceIdx, int componentIdx, float amp);

   static constexpr int MAX_VOICES = 16;

private:
   // std::string m_name;
   friend class base::instruments::KitInstrumentCopyer;
   std::vector<KitVoice> m_voices;
   std::optional<int> toVoiceIndex(int note) const;
   Ret<KitComponent*> getComponent(int voiceIdx, int componentIdx) noexcept;
   Ret<const KitComponent*> getConstComponent(int voiceIdx, int componentIdx) const noexcept;
};

}   // namespace base::instruments::loader

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H
