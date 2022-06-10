#ifndef SOUND_PARAMETER_STORAGE_H
#define SOUND_PARAMETER_STORAGE_H

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "CallbackSignal.h"
#include "LFO.h"
#include "ParameterPart.h"
#include "SoundSection.h"

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
   inline ParameterStorage(const description::sound::Section& rSoundSection);
   template <typename T>
   void setCommandedValuesOfVoice(int voiceId, const T& container) noexcept;
   inline void incSoundParameterValue(int voiceId, int parameterId,
                                      float increment,
                                      bool roundRobin = false) noexcept;
   inline void setSoundParameterValue(int voiceId, int parameterId, float value,
                                      bool roundRobin = false) noexcept;
   inline void setSoundParameterActualValue(int voiceId, int parameterId,
                                            float value) noexcept;

   inline void resetToInitialValue(int voiceIdx, int paramIdx) noexcept;
   inline void resetToInitialValues(int voiceIdx) noexcept;
   inline void resetToInitialValues() noexcept;

   inline std::optional<std::string> getActualPresetOfVoice(
       int voiceIdx) const noexcept;
   inline void setActualPresetOfVoice(int voiceIdx,
                                      const std::string& presetName) noexcept;

   template <typename Cb> void updateActualValues(Cb&& cb) noexcept;

   inline void markAllDirty() noexcept;

   inline float getCommandedValue(int voiceIdx, int parameterId, ParameterPart parameterPart = ParameterPart::Commanded) const noexcept;
   inline std::vector<float> getCommandedValuesOfVoice(
       int voiceIdx) const noexcept;

   constexpr static int ALL = -1;
   inline void uiShowsInterestInParameter(int voiceId,
                                          int parameterId = ALL) noexcept;
   inline void uiLoosesInterestInParameter(int voiceId,
                                           int parameterId = ALL) noexcept;
   inline void setWaveform(int voiceId, int parameterId,
                           lfo::Waveform waveform) noexcept;
   inline void setAmplitude(int voiceId, int parameterId,
                            float amplitude) noexcept;
   inline void setFrequency(int voiceId, int parameterId,
                            float frequency) noexcept;
   inline void setMultiplierExp(int voiceId, int parameterId,
                                uint32_t multiplierExp) noexcept;

   [[nodiscard]] inline lfo::Waveform waveform(int voiceId,
                                               int parameterId) const noexcept;
   [[nodiscard]] inline float amplitude(int voiceId,
                                        int parameterId) const noexcept;
   [[nodiscard]] inline float frequency(int voiceId,
                                        int parameterId) const noexcept;
   [[nodiscard]] inline uint32_t multiplierExp(int voiceId,
                                               int parameterId) const noexcept;

   inline void applyModifier(int voiceIndex, int paramIdx,
                             ParameterPart parameterPart, float destValue,
                             float intensity) noexcept;

   CB_SIGNAL(LFOWaveformChanged, int, int, lfo::Waveform);
   CB_SIGNAL(LFOAmplitudeChanged, int, int, float);
   CB_SIGNAL(LFOFrequencyChanged, int, int, float);
   CB_SIGNAL(LFOMultiplierExpChanged, int, int, uint32_t);
   CB_SIGNAL(ActualPresetChanged, int, const std::string&);

   struct Element
   {
      inline Element(bool isListIndex, int resolution) noexcept;
      static constexpr float FUZZ = 0.00001f;

      bool enabled{true};
      float commanded{0};
      float modifier{0};
      lfo::LFO lfo;
      float actual{-1};
      bool dirtyFlagRt{false};
      bool dirtyFlagUi{true};
      int uiInterestCount{0};

      inline std::optional<std::pair<float, float>>
      uiAsksForChangedValues() noexcept;
      inline std::optional<float> updateActualValue() noexcept;
      inline void setActualValueUnsynced(float value) noexcept;
      inline void setActualValue(float value) noexcept;
      inline void setCommandedValue(float value, bool markDirtyRt = true,
                                    bool roundRobin = false) noexcept;
      inline void incCommandedValue(float increment,
                                    bool roundRobin = false) noexcept;
      inline void applyModifier(float destination, float intensity,
                                ParameterPart parameterPart) noexcept;

   private:
      const bool m_isListIndex;
      const int m_resolution;
      float m_cachedLfoValue{0.0};
      inline float calcModified() const noexcept;
   };

   inline const Element& parameter(int voiceIdx, int paramIdx) const;
   inline Element& parameter(int voiceIdx, int paramIdx);

   template <typename Cb> void forEachParameter(Cb&& cb) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb) noexcept;
   static constexpr int GLOBAL =
       base::musicDevice::description::sound::GlobalSectionId;
   template <typename Cb>
   void forEachParameter(Cb&& cb, int voiceId) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb, int voiceId) noexcept;

   template <typename Cb> void forEachElementContainer(Cb&& cb);

   inline int paramCount(int voiceIdx) const noexcept;

private:
   const description::sound::Section& m_rSoundSection;
   inline lfo::LFO& lfoOf(int voiceId, int parameterId) noexcept;
   inline const lfo::LFO& lfoOf(int voiceId, int parameterId) const noexcept;
   struct EngineData
   {
      std::vector<Element> parameters;
      std::optional<std::string> actualPreset;
   };
   EngineData m_globalData;
   std::vector<EngineData> m_voicesData;

   inline const EngineData& elementContainer(int voiceIdx) const noexcept;
   inline EngineData& elementContainer(int voiceIdx) noexcept;
   inline void resize() noexcept;
};

}   // namespace sound
}   // namespace base::musicDevice

#include "SoundParameterStorage.inl"

#endif
