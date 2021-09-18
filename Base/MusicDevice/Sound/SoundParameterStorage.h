#ifndef SOUND_PARAMETER_STORAGE_H
#define SOUND_PARAMETER_STORAGE_H

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "LFO.h"
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
   void setParameterOfVoice(int voiceId, const T& container) noexcept;
   inline void setSoundParameterValue(int voiceId, int parameterId,
                                      float value) noexcept;
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

   inline float getCommandedValue(int voiceIdx, int parameterId) const noexcept;
   inline std::vector<float> getCommandedValuesOfVoice(
       int voiceIdx) const noexcept;

   constexpr static int ALL = -1;
   inline void uiShowsInterestInParameter(int voiceId,
                                          int parameterId = ALL) noexcept;
   inline void uiLoosesInterestInParameter(int voiceId,
                                           int parameterId = ALL) noexcept;

   inline LFO& lfoOf(int voiceId, int parameterId) noexcept;
   inline const LFO& lfoOf(int voiceId, int parameterId) const noexcept;

   struct Element
   {
      static constexpr int NUM_MODIFIERS = 10;
      struct Modifier
      {
         float destinationValue{0};
         float intensity{0};
      };

      float commanded{0};
      std::array<std::optional<Modifier>, NUM_MODIFIERS> modifiers;
      LFO lfo;
      float actual{-1};
      bool dirtyFlagRt{false};
      bool dirtyFlagUi{true};
      int uiInterestCount{0};

      inline std::optional<std::pair<float, float>>
      uiAsksForChangedValues() noexcept;
      inline bool updateActualValue() noexcept;
      inline void setActualValue(float value) noexcept;
      inline void setCommandedValue(float value) noexcept;

   private:
      float m_cachedLfoValue{0.0};
      inline float calcModified() const noexcept;
   };

   template <typename Cb> void forEachParameter(Cb&& cb) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb) noexcept;
   static constexpr int GLOBAL =
       base::musicDevice::description::sound::GlobalSectionId;
   template <typename Cb>
   void forEachParameter(Cb&& cb, int voiceId) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb, int voiceId) noexcept;

   template <typename Cb> void forEachElementContainer(Cb&& cb);

private:
   const description::sound::Section& m_rSoundSection;
   struct EngineData
   {
      std::vector<Element> parameters;
      std::optional<std::string> actualPreset;
   };
   EngineData m_globalData;
   std::vector<EngineData> m_voicesData;

   inline void resize() noexcept;
   inline const EngineData& elementContainer(int voiceIdx) const noexcept;
   inline EngineData& elementContainer(int voiceIdx) noexcept;
};

}   // namespace sound
}   // namespace base::musicDevice

#include "SoundParameterStorage.inl"

#endif