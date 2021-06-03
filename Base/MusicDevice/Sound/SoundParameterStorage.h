#ifndef SOUND_PARAMETER_STORAGE_H
#define SOUND_PARAMETER_STORAGE_H

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "LFO.h"

namespace base::musicDevice
{
namespace description::sound { struct Section; }
namespace sound
{
class ParameterStorage
{
public:
   inline void resizeBy(const description::sound::Section&
                           soundSection) noexcept;
   template<typename T>
   void setParameterOfVoice(int voiceId, const T& container) noexcept;
   inline void setSoundParameterValue(int voiceId, int parameterId,
                                      float value) noexcept;
   inline void setSoundParameterActualValue(int voiceId, int parameterId,
                                            float value) noexcept;

   template<typename Cb>
   void forEachParametersActualValue(Cb&& cb) const noexcept;

   inline void resetToInitialValues(
      int voiceIdx, const base::musicDevice::description::sound::Section&
                       soundSection) noexcept;

   inline std::optional<std::string> getActualPresetOfVoice(
      int voiceIdx) const noexcept;
   inline void setActualPresetOfVoice(int voiceIdx,
                                      const std::string& presetName) noexcept;

   template<typename Cb>
   void updateActualValues(Cb&& cb) noexcept;

   inline void markAllDirty() noexcept;

   inline std::vector<float> getCommandedValuesOfVoice(
      int voiceIdx) const noexcept;

   using ParamChangeCb = std::function<void(int voiceId, int paramIdx,
                                            float commanded, float actual)>;
   inline void registerParamChangeCbUI(ParamChangeCb cb);

   constexpr static int ALL = -2;
   inline void uiShowsInterestInParameter(int voiceId, int parameterId = ALL) noexcept;
   inline void uiLoosesInterestInParameter(int voiceId, int parameterId = ALL) noexcept;
   inline void updateUI() noexcept;
private:
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
      float actual{0};
      bool dirtyFlagRt{false};
      bool dirtyFlagUi{true};
      int uiInterestCount{0};

      inline bool updateActualValue() noexcept;
      inline void setActualValue(float value) noexcept;
      inline void setCommandedValue(float value) noexcept;

   private:
      float m_cachedLfoValue{0.0};
      inline float calcModified() const noexcept;
   };
   struct EngineData
   {
      std::vector<Element> parameters;
      std::optional<std::string> actualPreset;
      ParamChangeCb m_parameterChangeCb;
   };
   EngineData m_globalData;
   std::vector<EngineData> m_voicesData;

   std::vector<ParamChangeCb> m_paramChangeCbsUI;

   template<typename Cb>
   void forEachParameter(Cb&& cb) const noexcept;
   template<typename Cb>
   void forEachParameter(Cb&& cb) noexcept;

   inline const EngineData& elementContainer(int voiceIdx) const noexcept;
   inline EngineData& elementContainer(int voiceIdx) noexcept;
};

} // namespace sound
} // namespace base::musicDevice

#include "SoundParameterStorage.inl"

#endif