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
            auto& inserted = m_globalData.parameters.emplace_back(
                true, sourceRanges->size());
            inserted.onActualChanged(
                [this, paramIdx](float oldVal, float newVal) {
                   emitActualChanged(-1, paramIdx, oldVal, newVal);
                });
         }
         else
         {
            auto& inserted = m_globalData.parameters.emplace_back(
                false, m_rSoundSection.global->parameters[paramIdx]
                           .getSourceResolution());
            inserted.onActualChanged(
                [this, paramIdx](float oldVal, float newVal) {
                   emitActualChanged(-1, paramIdx, oldVal, newVal);
                });
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
            auto& inserted = m_voicesData[voiceIdx].parameters.emplace_back(
                true, sourceRanges->size());
            inserted.onActualChanged(
                [this, voiceIdx, paramIdx](float oldVal, float newVal) {
                   emitActualChanged(voiceIdx, paramIdx, oldVal, newVal);
                });
         }
         else
         {
            auto& inserted = m_voicesData[voiceIdx].parameters.emplace_back(
                false, engineDescr.parameters[paramIdx].getSourceResolution());
            inserted.onActualChanged(
                [this, voiceIdx, paramIdx](float oldVal, float newVal) {
                   emitActualChanged(voiceIdx, paramIdx, oldVal, newVal);
                });
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
                                                     IncrementMode incrementMode) noexcept
{
   elementContainer(voiceIdx).parameters[parameterIdx].incCommandedValue(
       increment, incrementMode);
}

inline void ParameterStorage::setSoundParameterValue(int voiceIdx,
                                                     int parameterIdx,
                                                     float value,
                                                     IncrementMode incrementMode) noexcept
{
   elementContainer(voiceIdx).parameters[parameterIdx].setCommandedValue(
       value, incrementMode);
}

inline void ParameterStorage::setSoundParameterActualValue(int voiceIdx,
                                                           int parameterIdx,
                                                           float value) noexcept
{
   assert(util::vector_index_in_range(parameterIdx,
                                      elementContainer(voiceIdx).parameters));
   elementContainer(voiceIdx).parameters[parameterIdx].setValueFromDeviceRel(
       value);
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

inline void ParameterStorage::_resetToInitialValue(
    int voiceIdx, int paramIdx, ParameterStorageElement& element) noexcept
{
   const auto& descr = m_rSoundSection.parameterDescription(voiceIdx, paramIdx);
   if (descr.role &&
       descr.role.value() ==
           description::sound::Parameter::Role::ComponentSelector &&
       element.isInSync())
   {
      return;
   }
   element.lfo().reset();
   const float initVal = m_rSoundSection.getInitialValueFor(voiceIdx, paramIdx);
   element.setCommandedValue(initVal, IncrementMode::Limit);
}

inline void ParameterStorage::resetToInitialValue(int voiceIdx,
                                                  int paramIdx) noexcept
{
   _resetToInitialValue(voiceIdx, paramIdx,
                        elementContainer(voiceIdx).parameters[paramIdx]);
}

inline void ParameterStorage::resetToInitialValues(int voiceIdx) noexcept
{
   forEachParameter(
       [this, voiceIdx](int paramIdx, ParameterStorageElement& element) {
          _resetToInitialValue(voiceIdx, paramIdx, element);
       },
       voiceIdx);
   elementContainer(voiceIdx).actualPreset.reset();
}

inline void ParameterStorage::resetToInitialValues() noexcept
{
   forEachParameter(
       [this](int voiceIdx, int paramIdx, ParameterStorageElement& element) {
          _resetToInitialValue(voiceIdx, paramIdx, element);
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

inline void ParameterStorage::updateActualValuesIfLfoActive() noexcept
{
   forEachParameter(
       [](int voiceIdx, int paramIdx, ParameterStorageElement& element) {
          element.calcActualValueIfLfoActive();
       });
}

inline float ParameterStorage::getCommandedValue(
    int voiceIdx, int parameterId, ParameterAttr parameterAttr) const noexcept
{
   switch (parameterAttr)
   {
      case ParameterAttr::Commanded:
      {
         return elementContainer(voiceIdx).parameters[parameterId].commanded();
      }
      case ParameterAttr::LfoAmplitude:
      {
         return amplitude(voiceIdx, parameterId);
      }
      case ParameterAttr::LfoFrequency:
      {
         return frequency(voiceIdx, parameterId);
      }
      case ParameterAttr::LfoWaveform:
      {
         return static_cast<float>(elementContainer(voiceIdx)
                                       .parameters[parameterId]
                                       .lfo()
                                       .waveform());
      }
      case ParameterAttr::LfoMultiplierExp:
      {
         return static_cast<float>(elementContainer(voiceIdx)
                                       .parameters[parameterId]
                                       .lfo()
                                       .multiplierExp());
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
      ret.push_back(parameter.commanded());
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
       [parameterId_](int paramIdx, ParameterStorageElement& element) {
          if (parameterId_ == ALL || parameterId_ == paramIdx)
          {
             element.incUiInterestCount();
          }
       },
       voiceId_);
}

inline void ParameterStorage::uiLoosesInterestInParameter(
    int voiceId_, int parameterId_) noexcept
{
   forEachParameter(
       [parameterId_](int paramIdx, ParameterStorageElement& element) {
          if (parameterId_ == ALL || parameterId_ == paramIdx)
          {
             element.decUiInterestCount();
          }
       },
       voiceId_);
}

inline int ParameterStorage::paramCount(int voiceIdx) const noexcept
{
   return elementContainer(voiceIdx).parameters.size();
}

inline const ParameterStorageElement& ParameterStorage::parameter(
    int voiceIdx, int paramIdx) const
{
   return elementContainer(voiceIdx).parameters[paramIdx];
}

inline ParameterStorageElement& ParameterStorage::parameter(int voiceIdx,
                                                            int paramIdx)
{
   return elementContainer(voiceIdx).parameters[paramIdx];
}

inline lfo::LFO& ParameterStorage::lfoOf(int voiceId, int parameterId) noexcept
{
   return parameter(voiceId, parameterId).lfo();
}

inline const lfo::LFO& ParameterStorage::lfoOf(int voiceId,
                                               int parameterId) const noexcept
{
   return parameter(voiceId, parameterId).lfo();
}

inline void ParameterStorage::setWaveform(int voiceId, int parameterId,
                                          lfo::Waveform waveform) noexcept
{
   lfoOf(voiceId, parameterId).setWaveform(waveform);
}

inline void ParameterStorage::setAmplitude(int voiceId, int parameterId,
                                           float amplitude) noexcept
{
   lfoOf(voiceId, parameterId).setAmplitude(amplitude);
}

inline void ParameterStorage::setFrequency(int voiceId, int parameterId,
                                           float frequency) noexcept
{
   lfoOf(voiceId, parameterId).setFrequency(frequency);
}

inline void ParameterStorage::setMultiplierExp(int voiceId, int parameterId,
                                               uint32_t multiplierExp) noexcept
{
   lfoOf(voiceId, parameterId).setMultiplierExp(multiplierExp);
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

inline void ParameterStorage::applyModifier(int voiceIdx, int paramIdx,
                                            ParameterAttr parameterAttr,
                                            float destValue,
                                            float intensity) noexcept
{
   elementContainer(voiceIdx).parameters[paramIdx].applyModifier(
       destValue, intensity, parameterAttr);
}

inline void ParameterStorage::resetModifier(int voiceIdx, int paramIdx,
                                            ParameterAttr parameterAttr)
{
   elementContainer(voiceIdx).parameters[paramIdx].resetModifier(parameterAttr);
}

inline void ParameterStorage::calcActualVal(int voiceIdx, int paramIdx)
{
   elementContainer(voiceIdx).parameters[paramIdx].calcActualVal();
}

inline
void ParameterStorage::calcActualValuesForVoice(int voiceIdx)
{
   forEachParameter(
       [](int paramIdx, ParameterStorageElement& element) {
          element.calcActualVal();
       },
       voiceIdx);
}

inline
void ParameterStorage::calcAllActualValues()
{
   forEachParameter(
       [](int, int, ParameterStorageElement& element) {
          element.calcActualVal();
       });
}

}   // namespace base::musicDevice::sound
