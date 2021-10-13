#include <loguru.hpp>

#include "SoundParameterStorage.h"

namespace base::musicDevice::sound
{
inline ParameterStorage::ParameterStorage(
    const description::sound::Section& rSoundSection) :
    m_rSoundSection(rSoundSection)
{
   resize();
}

inline void ParameterStorage::resize() noexcept
{
   if (m_rSoundSection.global)
   {
      m_globalData.parameters.reserve(
          m_rSoundSection.global->parameters.size());
      for (int paramIdx = 0;
           paramIdx < m_rSoundSection.global->parameters.size(); ++paramIdx)
      {
         if (m_rSoundSection.global->parameters[paramIdx].type ==
             description::sound::Parameter::Type::List)
         {
            const auto& sourceRanges =
                m_rSoundSection.global->parameters[paramIdx]
                    .source.sourceRanges;
            assert(sourceRanges.has_value());
            m_globalData.parameters.emplace_back(true, sourceRanges->size());
         }
         else
         {
            m_globalData.parameters.emplace_back(
                false, m_rSoundSection.global->parameters[paramIdx]
                           .getSourceResolution());
         }
      }
   }
   m_voicesData.resize(m_rSoundSection.voices.size());
   for (int voiceIdx = 0; voiceIdx < m_rSoundSection.voices.size(); ++voiceIdx)
   {
      const auto& engineDescr =
          m_rSoundSection.engines[m_rSoundSection.voices[voiceIdx].engineId];
      m_voicesData[voiceIdx].parameters.reserve(engineDescr.parameters.size());
      for (int paramIdx = 0; paramIdx < engineDescr.parameters.size();
           ++paramIdx)
      {
         if (engineDescr.parameters[paramIdx].type ==
             description::sound::Parameter::Type::List)
         {
            const auto& sourceRanges =
                engineDescr.parameters[paramIdx].source.sourceRanges;
            assert(sourceRanges.has_value());
            m_voicesData[voiceIdx].parameters.emplace_back(
                true, sourceRanges->size());
         }
         else
         {
            m_voicesData[voiceIdx].parameters.emplace_back(
                false, engineDescr.parameters[paramIdx].getSourceResolution());
         }
      }
   }
}

template <typename T>
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
   assert(util::vector_index_in_range(parameterIdx,
                                      elementContainer(voiceIdx).parameters));
   elementContainer(voiceIdx).parameters[parameterIdx].setActualValue(value);
}

template <typename Cb>
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

template <typename Cb> void ParameterStorage::forEachParameter(Cb&& cb) noexcept
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

template <typename Cb>
void ParameterStorage::forEachParameter(Cb&& cb, int voiceId) const noexcept
{
   if (voiceId == GLOBAL)
   {
      for (int paramIdx = 0; paramIdx < m_globalData.parameters.size();
           ++paramIdx)
      {
         cb(paramIdx, m_globalData.parameters[paramIdx]);
      }
   }
   else
   {
      const int voiceIdx = voiceId;
      for (int paramIdx = 0;
           paramIdx < m_voicesData[voiceIdx].parameters.size(); ++paramIdx)
      {
         cb(paramIdx, m_voicesData[voiceIdx].parameters[paramIdx]);
      }
   }
}

template <typename Cb>
void ParameterStorage::forEachParameter(Cb&& cb, int voiceId) noexcept
{
   if (voiceId == GLOBAL)
   {
      for (int paramIdx = 0; paramIdx < m_globalData.parameters.size();
           ++paramIdx)
      {
         cb(paramIdx, m_globalData.parameters[paramIdx]);
      }
   }
   else
   {
      const int voiceIdx = voiceId;
      for (int paramIdx = 0;
           paramIdx < m_voicesData[voiceIdx].parameters.size(); ++paramIdx)
      {
         cb(paramIdx, m_voicesData[voiceIdx].parameters[paramIdx]);
      }
   }
}

template <typename Cb> void ParameterStorage::forEachElementContainer(Cb&& cb)
{
   for (int paramIdx = 0; paramIdx < m_globalData.parameters.size(); ++paramIdx)
   {
      cb(m_globalData);
   }
   for (int voiceIdx = 0; voiceIdx < m_voicesData.size(); ++voiceIdx)
   {
      cb(m_voicesData[voiceIdx]);
   }
}

inline void ParameterStorage::resetToInitialValue(int voiceIdx,
                                                  int paramIdx) noexcept
{
   auto& element = elementContainer(voiceIdx).parameters[paramIdx];
   for (auto& e : element.modifiers) { e.reset(); }
   element.lfo.reset();
   const auto& descr = m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
   element.setCommandedValue(
       m_rSoundSection.getInitialValueFor(voiceIdx, paramIdx));
}

inline void ParameterStorage::resetToInitialValues(int voiceIdx) noexcept
{
   forEachParameter(
       [this, voiceIdx](int paramIdx, Element& element) {
          for (auto& e : element.modifiers) { e.reset(); }
          element.lfo.reset();
          const auto& descr =
              m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
          element.setCommandedValue(
              m_rSoundSection.getInitialValueFor(voiceIdx, paramIdx));
       },
       voiceIdx);

   elementContainer(voiceIdx).actualPreset.reset();
}

inline void ParameterStorage::resetToInitialValues() noexcept
{
   forEachParameter([this](int voiceId, int paramIdx, Element& element) {
      for (auto& e : element.modifiers) { e.reset(); }
      element.lfo.reset();
      const auto& descr = m_rSoundSection.parameterDescr(voiceId, paramIdx);
      element.setCommandedValue(
          m_rSoundSection.getInitialValueFor(voiceId, paramIdx));
   });
   forEachElementContainer(
       [](EngineData& engineData) { engineData.actualPreset.reset(); });
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

template <typename Cb>
void ParameterStorage::updateActualValues(Cb&& cb) noexcept
{
   forEachParameter([cb](int voiceIdx, int paramIdx, Element& element) {
      const bool changed = element.updateActualValue();
      if (changed)
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

inline float ParameterStorage::getCommandedValue(int voiceIdx,
                                                 int parameterId) const noexcept
{
   return elementContainer(voiceIdx).parameters[parameterId].commanded;
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

inline void ParameterStorage::uiShowsInterestInParameter(
    int voiceId_, int parameterId_) noexcept
{
   forEachParameter(
       [parameterId_](int paramIdx, Element& element) {
          if (parameterId_ == ALL || parameterId_ == paramIdx)
          {
             element.dirtyFlagUi = true;
             element.uiInterestCount++;
          }
       },
       voiceId_);
}

inline void ParameterStorage::uiLoosesInterestInParameter(
    int voiceId_, int parameterId_) noexcept
{
   forEachParameter(
       [parameterId_](int paramIdx, Element& element) {
          if (parameterId_ == ALL || parameterId_ == paramIdx)
          {
             element.uiInterestCount--;
          }
       },
       voiceId_);
}

inline LFO& ParameterStorage::lfoOf(int voiceId, int parameterId) noexcept
{
   return elementContainer(voiceId).parameters[parameterId].lfo;
}

inline const LFO& ParameterStorage::lfoOf(int voiceId,
                                          int parameterId) const noexcept
{
   return elementContainer(voiceId).parameters[parameterId].lfo;
}

inline ParameterStorage::Element::Element(bool isListIndex,
                                          int resolution) noexcept :
    m_isListIndex(isListIndex), m_resolution(resolution)
{
}

inline std::optional<std::pair<float, float>>
ParameterStorage::Element::uiAsksForChangedValues() noexcept
{
   if (dirtyFlagUi && uiInterestCount)
   {
      dirtyFlagUi = false;
      return std::make_pair(commanded, actual);
   }
   return std::nullopt;
}

inline bool
ParameterStorage::Element::updateActualValue() noexcept
{
   if (!dirtyFlagRt && !lfo.enabled())   // performance improving shortcut
   {
      return false;
   }
   float actualBefore = actual;
   actual             = calcModified();
   const float range  = m_isListIndex ? m_resolution : 1.0;
   if (lfo.enabled())
   {
      m_cachedLfoValue = lfo.calculateValue() * range;
      actual += m_cachedLfoValue;
      actual = std::min(range - FUZZ, actual);
      actual = std::max(0.0f, actual);
   }
   else
   {
      m_cachedLfoValue = 0.0;
   }

   dirtyFlagRt = false;
   if (m_isListIndex)
   {
      if (int(actualBefore) != int(actual))
      {
         //LOG_F(INFO, "actualBefore {} actual {}", actualBefore, actual);
         dirtyFlagUi = true;
         return true;
      }
   }
   else
   {
      if (int(actualBefore * m_resolution) != int(actual * m_resolution)
          //|| actual == 0
          )
      {
         dirtyFlagUi = true;
         return true;
      }
   }
   return false;
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
   const float range = m_isListIndex ? m_resolution : 1.0;
   if (value < 0.0)
   {
      value = 0.0;
   }
   if(value >= range)
   {
      value = range - FUZZ;
   }
   commanded   = value;
   dirtyFlagRt = true;
   dirtyFlagUi = true;
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

}   // namespace base::musicDevice::sound