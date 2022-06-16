#include <spdlog/spdlog.h>

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
                    .source.midi->sourceRanges;
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
                engineDescr.parameters[paramIdx].source.midi->sourceRanges;
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
void ParameterStorage::setCommandedValuesOfVoice(int voiceIdx,
                                                 const T& container) noexcept
{
   assert(elementContainer(voiceIdx).parameters.size() == container.size());
   for (int i = 0; i < container.size(); ++i)
   {
      elementContainer(voiceIdx).parameters[i].setCommandedValue(container[i]);
   }
}

inline void ParameterStorage::incSoundParameterValue(int voiceIdx,
                                                     int parameterIdx,
                                                     float increment,
                                                     bool roundRobin) noexcept
{
   elementContainer(voiceIdx).parameters[parameterIdx].incCommandedValue(
       increment, roundRobin);
}

inline void ParameterStorage::setSoundParameterValue(int voiceIdx,
                                                     int parameterIdx,
                                                     float value,
                                                     bool roundRobin) noexcept
{
   elementContainer(voiceIdx).parameters[parameterIdx].setCommandedValue(
       value, roundRobin);
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
      cb(m_globalData, GLOBAL);
   }
   for (int voiceIdx = 0; voiceIdx < m_voicesData.size(); ++voiceIdx)
   {
      cb(m_voicesData[voiceIdx], voiceIdx);
   }
}

inline void ParameterStorage::resetToInitialValue(int voiceIdx,
                                                  int paramIdx) noexcept
{
   auto& element    = elementContainer(voiceIdx).parameters[paramIdx];
   element.modifier = 0;
   element.lfo.reset();
   const auto& descr = m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
   if (descr.role &&
       descr.role.value() ==
           description::sound::Parameter::Role::ComponentSelector &&
       element.actual != -1)
      return;
   const float initVal = m_rSoundSection.getInitialValueFor(voiceIdx, paramIdx);
   element.actual      = -1;
   element.dirtyFlagRt = true;
   element.setCommandedValue(initVal);
}

inline void ParameterStorage::resetToInitialValues(int voiceIdx) noexcept
{
   forEachParameter(
       [this, voiceIdx](int paramIdx, Element& element) {
          element.modifier = 0;
          element.lfo.reset();
          const auto& descr =
              m_rSoundSection.parameterDescr(voiceIdx, paramIdx);
          if (descr.role &&
              descr.role.value() ==
                  description::sound::Parameter::Role::ComponentSelector &&
              element.actual != -1)
             return;
          const float initVal =
              m_rSoundSection.getInitialValueFor(voiceIdx, paramIdx);
          element.actual      = -1;
          element.dirtyFlagRt = true;
          element.setCommandedValue(initVal);
       },
       voiceIdx);

   elementContainer(voiceIdx).actualPreset.reset();
}

inline void ParameterStorage::resetToInitialValues() noexcept
{
   forEachParameter([this](int voiceId, int paramIdx, Element& element) {
      element.modifier = 0;
      element.lfo.reset();
      const auto& descr = m_rSoundSection.parameterDescr(voiceId, paramIdx);
      if (descr.role &&
          descr.role.value() ==
              description::sound::Parameter::Role::ComponentSelector &&
          element.actual != -1)
         return;
      const float initVal =
          m_rSoundSection.getInitialValueFor(voiceId, paramIdx);
      element.actual      = -1;
      element.dirtyFlagRt = true;
      element.setCommandedValue(initVal);
   });
   forEachElementContainer([](EngineData& engineData, int voiceIdx) {
      engineData.actualPreset.reset();
   });
}

inline std::optional<std::string> ParameterStorage::getActualPresetOfVoice(
    int voiceIdx) const noexcept
{
   return elementContainer(voiceIdx).actualPreset;
}

inline void ParameterStorage::setActualPresetOfVoice(
    int voiceIdx, const std::string& presetName) noexcept
{
   if (elementContainer(voiceIdx).actualPreset != presetName)
   {
      elementContainer(voiceIdx).actualPreset = presetName;
      emitActualPresetChanged(voiceIdx, presetName);
   }
}

template <typename Cb>
void ParameterStorage::updateActualValues(Cb&& cb) noexcept
{
   forEachParameter([cb](int voiceIdx, int paramIdx, Element& element) {
      const auto prevVal = element.updateActualValue();
      if (prevVal)
      {
         cb(voiceIdx, paramIdx, element.actual, *prevVal);
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
                                                 int parameterId,
                                                 ParameterPart parameterPart) const noexcept
{
   switch(parameterPart)
   {
      case ParameterPart::Commanded:
      {
         return elementContainer(voiceIdx).parameters[parameterId].commanded;
      }
      case ParameterPart::LfoAmplitude:
      {
         return amplitude(voiceIdx, parameterId);
      }
      case ParameterPart::LfoFrequency:
      {
         return frequency(voiceIdx, parameterId);
      }
      case ParameterPart::LfoWaveform:
      {
         return elementContainer(voiceIdx).parameters[parameterId].lfo.waveformAsFloat();
      }
      case ParameterPart::LfoMultiplierExp:
      {
         return elementContainer(voiceIdx).parameters[parameterId].lfo.multiplierExpAsFloat();
      }
   }
   return 0;
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

inline const ParameterStorage::Element& ParameterStorage::parameter(
    int voiceIdx, int paramIdx) const
{
   return elementContainer(voiceIdx).parameters[paramIdx];
}

inline ParameterStorage::Element& ParameterStorage::parameter(int voiceIdx,
                                                              int paramIdx)
{
   return elementContainer(voiceIdx).parameters[paramIdx];
}

inline lfo::LFO& ParameterStorage::lfoOf(int voiceId, int parameterId) noexcept
{
   return parameter(voiceId, parameterId).lfo;
}

inline const lfo::LFO& ParameterStorage::lfoOf(int voiceId,
                                               int parameterId) const noexcept
{
   return parameter(voiceId, parameterId).lfo;
}

inline int ParameterStorage::paramCount(int voiceIdx) const noexcept
{
   return elementContainer(voiceIdx).parameters.size();
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

inline void ParameterStorage::Element::applyModifier(
    float destination, float intensity, ParameterPart parameterPart) noexcept
{
   switch (parameterPart)
   {
      case ParameterPart::Commanded:
      {
         modifier += intensity * (destination - commanded);
         break;
      }
      case ParameterPart::LfoAmplitude:
      {
         lfo.applyModifier2Amplitude(destination, intensity);
         break;
      }
      case ParameterPart::LfoFrequency:
      {
         lfo.applyModifier2Frequency(destination, intensity);
         break;
      }
      case ParameterPart::LfoWaveform:
      {
         lfo.applyModifier2Waveform(destination, intensity);
         break;
      }
      case ParameterPart::LfoMultiplierExp:
      {
         lfo.applyModifier2MultiplierExp(destination, intensity);
         break;
      }
   }
   dirtyFlagRt = true;
}

inline std::optional<float>
ParameterStorage::Element::updateActualValue() noexcept
{
   if (lfo.getAndResetJustGotDisabled())
      dirtyFlagRt = true;
   if (!enabled || (!dirtyFlagRt && !lfo.enabled()))
   {
      return std::nullopt;
   }
   float actualBefore = actual;
   actual             = commanded + modifier;
   modifier = 0;
   const float range  = m_isListIndex ? m_resolution : 1.0;
   if (lfo.enabled())
   {
      m_cachedLfoValue = lfo.calculateValue() * range;
      actual += m_cachedLfoValue;
      actual = util::clip(actual, 0.0f, range);
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
         // spdlog::info( "actualBefore {} actual {}", actualBefore, actual);
         dirtyFlagUi = true;
         return actualBefore;
      }
   }
   else
   {
      if (int(actualBefore * m_resolution) != int(actual * m_resolution)
          //|| actual == 0
      )
      {
         dirtyFlagUi = true;
         return actualBefore;
      }
   }
   return std::nullopt;
}

inline void ParameterStorage::Element::setActualValueUnsynced(
    float value) noexcept
{
   actual      = value;
   dirtyFlagUi = true;
}

inline void ParameterStorage::Element::setActualValue(float value) noexcept
{
   actual      = value;
   commanded   = actual - (m_cachedLfoValue + modifier);
   dirtyFlagUi = true;
}

inline void ParameterStorage::Element::setCommandedValue(
    float value, bool markDirtyRt, bool roundRobin) noexcept
{
   const float range = m_isListIndex ? m_resolution : 1.0;
   if (roundRobin)
   {
      if (value < 0.0)
      {
         value = m_isListIndex ? range - 1 : range - FUZZ;
      }
      if (value >= range)
      {
         value = 0.0;
      }
   }
   else
   {
      if (value < 0.0)
      {
         value = 0.0;
      }
      if (value >= range)
      {
         value = m_isListIndex ? range - 1 : range - FUZZ;
      }
   }
   commanded   = value;
   dirtyFlagRt = markDirtyRt;
   dirtyFlagUi = true;
}

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

inline void ParameterStorage::Element::incCommandedValue(
    float increment, bool roundRobin) noexcept
{
   const float theIncrement = m_isListIndex ? sgn(increment) : increment;
   setCommandedValue(commanded + theIncrement, true, roundRobin);
}

inline void ParameterStorage::setWaveform(int voiceId, int parameterId,
                                          lfo::Waveform waveform) noexcept
{
   if (lfoOf(voiceId, parameterId).setWaveform(waveform))
   {
      emitLFOWaveformChanged(voiceId, parameterId, waveform);
   }
}

inline void ParameterStorage::setAmplitude(int voiceId, int parameterId,
                                           float amplitude) noexcept
{
   if (lfoOf(voiceId, parameterId).setAmplitude(amplitude))
   {
      emitLFOAmplitudeChanged(voiceId, parameterId, amplitude);
   }
}

inline void ParameterStorage::setFrequency(int voiceId, int parameterId,
                                           float frequency) noexcept
{
   if (lfoOf(voiceId, parameterId).setFrequency(frequency))
   {
      emitLFOFrequencyChanged(voiceId, parameterId, frequency);
   }
}

inline void ParameterStorage::setMultiplierExp(int voiceId, int parameterId,
                                               uint32_t multiplierExp) noexcept
{
   if (lfoOf(voiceId, parameterId).setMultiplierExp(multiplierExp))
   {
      emitLFOMultiplierExpChanged(voiceId, parameterId, multiplierExp);
   }
}

inline lfo::Waveform ParameterStorage::waveform(int voiceId,
                                                int parameterId) const noexcept
{
   return lfoOf(voiceId, parameterId).waveform();
}

inline float ParameterStorage::amplitude(int voiceId,
                                         int parameterId) const noexcept
{
   return lfoOf(voiceId, parameterId).amplitude();
}

inline float ParameterStorage::frequency(int voiceId,
                                         int parameterId) const noexcept
{
   return lfoOf(voiceId, parameterId).frequency();
}

inline uint32_t ParameterStorage::multiplierExp(int voiceId,
                                                int parameterId) const noexcept
{
   return lfoOf(voiceId, parameterId).multiplierExp();
}

inline void ParameterStorage::applyModifier(int voiceIndex, int paramIdx,
                                            ParameterPart parameterPart,
                                            float destValue,
                                            float intensity) noexcept
{
   elementContainer(voiceIndex).parameters[paramIdx].applyModifier(destValue, intensity,
                                                        parameterPart);
}

}   // namespace base::musicDevice::sound
