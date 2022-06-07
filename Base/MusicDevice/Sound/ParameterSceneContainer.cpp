#include "ParameterSceneContainer.h"
#include "VectorIndexInRange.h"

#define CHECK_IN_RANGE(sceneIdx) \
   if(!util::vector_index_in_range(sceneIdx, m_data))   \
   {                                                    \
      spdlog::error("Index out of range: {}", sceneIdx);\
      return;                                           \
   }

using namespace base::musicDevice::sound;
ParameterSceneContainer::ParameterSceneContainer() :
    m_memoryPool("ParameterSceneContainer")
{
   m_data.reserve(MAX_NUM_SCENES);
   for(int i = 0; i < MAX_NUM_SCENES; ++i)
   {
      m_data.emplace_back(m_memoryPool.pool());
   }
}

void ParameterSceneContainer::setSceneName(int sceneIdx,
                                           std::string_view name) noexcept
{
   CHECK_IN_RANGE(sceneIdx);
   if (m_data[sceneIdx].name != name)
   {
      m_data[sceneIdx].name = name;
      emitSceneNameChanged(sceneIdx, std::string(name));
   }
}

void ParameterSceneContainer::setSceneIntensity(int sceneIdx,
                                                float intensity) noexcept
{
   CHECK_IN_RANGE(sceneIdx);
   if (m_data[sceneIdx].intensity != intensity)
   {
      m_data[sceneIdx].intensity = intensity;
      emitSceneIntensityChanged(sceneIdx, intensity);
   }
}

void ParameterSceneContainer::setModifierEndValue(
    int sceneIdx, const ParameterCoordinate& paramCoord, float value) noexcept
{
   CHECK_IN_RANGE(sceneIdx);
   auto modIt = std::find_if(m_data[sceneIdx].modifiers.begin(), m_data[sceneIdx].modifiers.end(),
                             [&](const ParameterScene::Modifier& m) {
                                return m.destParamCoord == paramCoord;
                             });
   if (modIt == m_data[sceneIdx].modifiers.end())
   {
      m_data[sceneIdx].modifiers.emplace_back(paramCoord, value);
      emitModifierEndValueChanged(sceneIdx, paramCoord, value);
   }
   else
   {
      if (modIt->goalValue != value)
      {
         modIt->goalValue = value;
         emitModifierEndValueChanged(sceneIdx, paramCoord, value);
      }
   }
}

void ParameterSceneContainer::removeModifier(
    int sceneIdx, const ParameterCoordinate& paramCoord) noexcept
{
   CHECK_IN_RANGE(sceneIdx);
   auto modIt = std::find_if(m_data[sceneIdx].modifiers.begin(), m_data[sceneIdx].modifiers.end(),
                             [&](const ParameterScene::Modifier& m) {
                                return m.destParamCoord == paramCoord;
                             });
   if (modIt != m_data[sceneIdx].modifiers.end())
   {
      m_data[sceneIdx].modifiers.erase(modIt);
      emitModifierRemoved(sceneIdx, paramCoord);
   }
}
