#include "SoundParameterStorage.h"
#include "SoundSection.h"

namespace base::musicDevice::sound
{
inline void ParameterStorage::resizeBy(
   const description::sound::Section& soundSection) noexcept
{
   if (soundSection.global)
   {
      m_globalData.parameters.resize(soundSection.global->parameters.size());
   }
   m_voicesData.resize(soundSection.voices.size());
   for (int voiceIdx = 0; voiceIdx < soundSection.voices.size(); ++voiceIdx)
   {
      const auto& engineDescr =
         soundSection.engines[soundSection.voices[voiceIdx].engineId];
      m_voicesData[voiceIdx].parameters.resize(engineDescr.parameters.size());
   }
}

template<typename T>
void ParameterStorage::setParameterOfVoice(int voiceIdx,
                                           const T& container) noexcept
{
   assert(elementContainer(voiceIdx).parameters.size() == container.size());
   for (int i = 0; i < container.size(); ++i)
   {
      elementContainer(voiceIdx).parameters[i].setCommandedValue(container[i]);
   }
}

inline void ParameterStorage::setSoundParameterValue(int voiceIdx,
                                                     int parameterIdx,
                                                     float value) noexcept
{
   elementContainer(voiceIdx).parameters[parameterIdx].setCommandedValue(value);
}

inline void ParameterStorage::setSoundParameterActualValue(int voiceIdx,
                                                           int parameterIdx,
                                                           float value) noexcept
{
   assert(util::vector_index_in_range(parameterIdx,  elementContainer(voiceIdx).parameters));
   elementContainer(voiceIdx).parameters[parameterIdx].setActualValue(value);
}

template<typename Cb>
void ParameterStorage::forEachParametersActualValue(Cb&& cb) const noexcept
{
   forEachParameter([cb](int voiceIdx, int paramIdx, const Element& element) {
      cb(voiceIdx, paramIdx, element.actual);
   });
}

template<typename Cb>
void ParameterStorage::forEachParameter(Cb&& cb) const noexcept
{
   for (int paramIdx = 0; paramIdx < m_globalData.parameters.size(); ++paramIdx)
   {
      cb(base::musicDevice::description::sound::GlobalSectionId, paramIdx,
         m_globalData.parameters[paramIdx]);
   }
   for (int voiceIdx = 0; voiceIdx < m_voicesData.size(); ++voiceIdx)
   {
      for (int paramIdx = 0;
           paramIdx < m_voicesData[voiceIdx].parameters.size(); ++paramIdx)
      {
         cb(voiceIdx, paramIdx, m_voicesData[voiceIdx].parameters[paramIdx]);
      }
   }
}

template<typename Cb>
void ParameterStorage::forEachParameter(Cb&& cb) noexcept
{
   for (int paramIdx = 0; paramIdx < m_globalData.parameters.size(); ++paramIdx)
   {
      cb(base::musicDevice::description::sound::GlobalSectionId, paramIdx,
         m_globalData.parameters[paramIdx]);
   }
   for (int voiceIdx = 0; voiceIdx < m_voicesData.size(); ++voiceIdx)
   {
      for (int paramIdx = 0;
           paramIdx < m_voicesData[voiceIdx].parameters.size(); ++paramIdx)
      {
         cb(voiceIdx, paramIdx, m_voicesData[voiceIdx].parameters[paramIdx]);
      }
   }
}

inline void ParameterStorage::resetToInitialValues(
   int voiceIdx,
   const base::musicDevice::description::sound::Section& soundSection) noexcept
{
   // TODO
}

inline std::optional<std::string> ParameterStorage::getActualPresetOfVoice(
   int voiceIdx) const noexcept
{
   return elementContainer(voiceIdx).actualPreset;
}

inline void ParameterStorage::setActualPresetOfVoice(
   int voiceIdx, const std::string& presetName) noexcept
{
   elementContainer(voiceIdx).actualPreset = presetName;
}

template<typename Cb>
void ParameterStorage::updateActualValues(Cb&& cb) noexcept
{
   forEachParameter([cb](int voiceIdx, int paramIdx, Element& element) {
      if (element.updateActualValue())
      {
         cb(voiceIdx, paramIdx, element.actual);
      }
   });
}

inline void ParameterStorage::markAllDirty() noexcept
{
   forEachParameter([](int voiceIdx, int paramIdx, Element& element) {
      element.dirtyFlagRt = true;
   });
}

inline std::vector<float> ParameterStorage::getCommandedValuesOfVoice(
   int voiceIdx) const noexcept
{
   std::vector<float> ret;
   for (const auto& parameter : elementContainer(voiceIdx).parameters)
   {
      ret.push_back(parameter.commanded);
   }
   return ret;
}

inline const ParameterStorage::EngineData& ParameterStorage::elementContainer(
   int voiceIdx) const noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == voiceIdx)
   {
      return m_globalData;
   }
   else
   {
      assert(util::vector_index_in_range(voiceIdx, m_voicesData));
      return m_voicesData[voiceIdx];
   }
}

inline ParameterStorage::EngineData& ParameterStorage::elementContainer(
   int voiceIdx) noexcept
{
   if (base::musicDevice::description::sound::GlobalSectionId == voiceIdx)
   {
      return m_globalData;
   }
   else
   {
      assert(util::vector_index_in_range(voiceIdx, m_voicesData));
      return m_voicesData[voiceIdx];
   }
}

inline void ParameterStorage::registerForParameterChange(
   int voiceId, ParamChangeCb cb) noexcept
{
   auto& voiceElement = elementContainer(voiceId);
   voiceElement.m_parameterChangeCb = cb;
   for(auto& paramDescr : voiceElement.parameters)
   {
      paramDescr.dirtyFlagUi = true;
   }
}

inline void ParameterStorage::unregisterForParameterChange(int voiceId) noexcept
{
   elementContainer(voiceId).m_parameterChangeCb = nullptr;
}

inline void ParameterStorage::updateUI() noexcept
{
   forEachParameter([this](int voiceIdx, int paramIdx, Element& element) {
      if (!element.dirtyFlagUi)
      {
         return;
      }
      if (elementContainer(voiceIdx).m_parameterChangeCb)
      {
         elementContainer(voiceIdx).m_parameterChangeCb(
            voiceIdx, paramIdx, element.commanded, element.actual);
         element.dirtyFlagUi = false;
      }
   });
}

inline bool ParameterStorage::Element::updateActualValue() noexcept
{
   if (!dirtyFlagRt)
   {
      return false;
   }
   actual = calcModified();
   if(lfo.enabled())
   {
      m_cachedLfoValue = lfo.calculateValue();
      actual += m_cachedLfoValue;
      actual = std::min(1.0f, actual);
      actual = std::max(0.0f, actual);
   }
   else
   {
      m_cachedLfoValue = 0.0;
   }
   
   dirtyFlagRt = false;
   return true;
}

inline void ParameterStorage::Element::setActualValue(float value) noexcept
{
   float A = 0;
   float B = 0;
   for (const auto& modifier : modifiers)
   {
      if (modifier)
      {
         A += modifier->intensity * modifier->destinationValue;
         B += modifier->intensity;
      }
   }
   actual      = value;
   commanded   = (actual - m_cachedLfoValue - A) / (1.0f - B);
   dirtyFlagUi = true;
}

inline void ParameterStorage::Element::setCommandedValue(float value) noexcept
{
   if (commanded != value)
   {
      commanded   = value;
      dirtyFlagRt = true;
      dirtyFlagUi = true;
   }
}

inline float ParameterStorage::Element::calcModified() const noexcept
{
   float ret = commanded;
   for (const auto& modifier : modifiers)
   {
      if (modifier && modifier->intensity)
      {
         ret += (modifier->destinationValue - commanded) * modifier->intensity;
      }
   }
   return ret;
}

} // namespace base::musicDevice::sound