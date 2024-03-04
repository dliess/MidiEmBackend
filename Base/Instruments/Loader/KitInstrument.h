#ifndef KIT_INSTRUMENT_LOADER_H
#define KIT_INSTRUMENT_LOADER_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "Instrument.h"
#include "MusicDeviceId.h"
#include "function_ref.h"
#include "ErrorHandling.h"
#include "ParameterData.h"


namespace base::instruments { class KitInstrumentCopyer; }

namespace base::instruments::loader
{

class KitInstrument : public Instrument
{
public:
   KitInstrument() = default;
   explicit KitInstrument(std::string name) noexcept;

      // TODO: maybe remove this
   // [[nodiscard]] Ret<float> getParameterValue(
   //     int voiceIdx, int componentIdx, int parameterIdx,
   //     musicDevice::sound::ParameterAttr parameterAttr) const;
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

   struct Voice   //: public util::Identifiable
   {
      struct Component
      {
         Component() = default;
         explicit Component(const std::vector<musicDevice::description::sound::Parameter>& paramDescr,
                               musicDevice::MusicDeviceId soundDeviceId,
                               int sdVoiceIdx) noexcept;

         bool operator==(const Component& rhs) const;

         const std::vector<musicDevice::description::sound::Parameter>* paramDescr{nullptr};
         std::vector<musicDevice::sound::ParameterData> parameterData;
         musicDevice::MusicDeviceId soundDeviceId;
         int sdVoiceIdx{0};
         int noteOffset{0};
         float amp{1.0f};
      };
      Voice() noexcept = default;
      explicit Voice(std::string name) noexcept : name(std::move(name)) {};
      std::string name;
      static constexpr int NUM_MAX_COMPONENTS_PER_VOICE = 4;
      std::vector<Component> components;
      int noteOffset{0};
      float amp{1.0f};
      bool operator==(const Voice& rhs) const noexcept = default;
   };
private:
   // std::string m_name;
   friend class base::instruments::KitInstrumentCopyer;
   std::vector<Voice> m_voices;
   std::optional<int> toVoiceIndex(int note) const;
   Ret<Voice::Component*> getComponent(int voiceIdx, int componentIdx) noexcept;
   Ret<const Voice::Component*> getConstComponent(int voiceIdx, int componentIdx) const noexcept;
};

}   // namespace base::instruments::loader

#include "KitInstrument.inl"
#include "KitInstrumentMeta.h"

#endif   // KIT_INSTRUMENT_H
