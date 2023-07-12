#ifndef SOUND_PARAMETER_STORAGE_H
#define SOUND_PARAMETER_STORAGE_H

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "LFO.h"
#include "ParameterAttr.h"
#include "SoundParameterStorageElement.h"
#include "SoundSection.h"
#include "CallbackSignal.h"

namespace base::musicDevice
{
namespace description::sound
{
struct Section;
}
namespace sound
{
class ParameterStorage
{
public:
   ParameterStorage(const description::sound::Section& rSoundSection);
   template <typename T>
   void setCommandedValuesOfVoice(int voiceId, const T& container) noexcept;
   void incSoundParameterValue(int voiceId, int parameterId,
                                      float increment,
                                      bool roundRobin = false) noexcept;
   void setSoundParameterValue(int voiceId, int parameterId, float value,
                                      bool roundRobin = false) noexcept;
   void setSoundParameterActualValue(int voiceId, int parameterId,
                                            float value) noexcept;

   void resetToInitialValue(int voiceIdx, int paramIdx) noexcept;
   void resetToInitialValues(int voiceIdx) noexcept;
   void resetToInitialValues() noexcept;

   std::optional<std::string> getActualPresetOfVoice(
       int voiceIdx) const noexcept;
   void setActualPresetOfVoice(int voiceIdx,
                                      const std::string& presetName) noexcept;

   void updateActualValuesIfLfoActive() noexcept;

   float getCommandedValue(
       int voiceIdx, int parameterId,
       ParameterAttr parameterAttr = ParameterAttr::Commanded) const noexcept;
   std::vector<float> getCommandedValuesOfVoice(
       int voiceIdx) const noexcept;

   constexpr static int ALL = -1;
   void uiShowsInterestInParameter(int voiceId,
                                          int parameterId = ALL) noexcept;
   void uiLoosesInterestInParameter(int voiceId,
                                           int parameterId = ALL) noexcept;
   void setWaveform(int voiceId, int parameterId,
                           lfo::Waveform waveform) noexcept;
   void setAmplitude(int voiceId, int parameterId,
                            float amplitude) noexcept;
   void setFrequency(int voiceId, int parameterId,
                            float frequency) noexcept;
   void setMultiplierExp(int voiceId, int parameterId,
                                uint32_t multiplierExp) noexcept;

   [[nodiscard]] lfo::Waveform waveform(int voiceId,
                                               int parameterId) const noexcept;
   [[nodiscard]] float amplitude(int voiceId,
                                        int parameterId) const noexcept;
   [[nodiscard]] float frequency(int voiceId,
                                        int parameterId) const noexcept;
   [[nodiscard]] uint32_t multiplierExp(int voiceId,
                                               int parameterId) const noexcept;

   void applyModifier(int voiceIdx, int paramIdx, ParameterAttr parameterAttr,
                      float destValue, float intensity) noexcept;
   void resetModifier(int voiceIdx, int paramIdx, ParameterAttr parameterAttr);
   void calcActualVal(int voiceIdx, int paramIdx);

   CB_SIGNAL(ActualPresetChanged, int, const std::string&);

   const ParameterStorageElement& parameter(int voiceIdx,
                                                   int paramIdx) const;
   ParameterStorageElement& parameter(int voiceIdx, int paramIdx);

   template <typename Cb> void forEachParameter(Cb&& cb) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb) noexcept;
   static constexpr int GLOBAL =
       base::musicDevice::description::sound::GlobalSectionId;
   template <typename Cb>
   void forEachParameter(Cb&& cb, int voiceId) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb, int voiceId) noexcept;

   template <typename Cb> void forEachElementContainer(Cb&& cb);

   int paramCount(int voiceIdx) const noexcept;

   CB_SIGNAL_SINGLE_SUBSCRIBER(ActualChanged, int, int, float, float);

private:
   const description::sound::Section& m_rSoundSection;
   lfo::LFO& lfoOf(int voiceId, int parameterId) noexcept;
   const lfo::LFO& lfoOf(int voiceId, int parameterId) const noexcept;
   struct EngineData
   {
      std::vector<ParameterStorageElement> parameters;
      std::optional<std::string> actualPreset;
   };
   EngineData m_globalData;
   std::vector<EngineData> m_voicesData;

   const EngineData& elementContainer(int voiceIdx) const noexcept;
   EngineData& elementContainer(int voiceIdx) noexcept;
   void resize() noexcept;

   void _resetToInitialValue(int voiceIdx, int paramIdx,
                                    ParameterStorageElement& element) noexcept;
};

}   // namespace sound
}   // namespace base::musicDevice

#include "SoundParameterStorage.inl"

#endif
