#include "ParameterSceneContainer.h"

using namespace base::musicDevice::sound;
ParameterSceneContainer::ParameterSceneContainer() :
    m_memoryPool("ParameterSceneContainer"), m_data(&m_memoryPool.pool())
{
}

void ParameterSceneContainer::setSceneName(const util::Identifiable::UUID& uuid,
                                           std::string_view name) noexcept
{
   auto it = std::find_if(
       m_data.begin(), m_data.end(),
       [uuid](const ParameterScene& ps) { return ps.id() == uuid; });
   if (it != m_data.end())
   {
      if (it->name != name)
      {
         it->name = name;
         emitSceneNameChanged(uuid, std::string(name));
      }
   }
   else
   {
      m_data.emplace_back(name, m_memoryPool.pool());
      emitSceneNameChanged(uuid, std::string(name));
   }
}

void ParameterSceneContainer::setSceneIntensity(
    const util::Identifiable::UUID& uuid, float intensity) noexcept
{
   auto it = std::find_if(
       m_data.begin(), m_data.end(),
       [uuid](const ParameterScene& ps) { return ps.id() == uuid; });
   if (it != m_data.end())
   {
      if (it->intensity != intensity)
      {
         it->intensity = intensity;
         emitSceneIntensityChanged(uuid, intensity);
      }
   }
   else
   {
      spdlog::error("No scene found with uuid: {}", util::uuid2Str(uuid));
   }
}

void ParameterSceneContainer::setModifierEndValue(
    const util::Identifiable::UUID& sceneUuid,
    const ParameterCoordinate& paramCoord, float value) noexcept
{
   auto it = std::find_if(
       m_data.begin(), m_data.end(),
       [sceneUuid](const ParameterScene& ps) { return ps.id() == sceneUuid; });
   if (it == m_data.end())
   {
      spdlog::error("No scene found with uuid: {}", util::uuid2Str(sceneUuid));
      return;
   }
   auto modIt = std::find_if(it->modifiers.begin(), it->modifiers.end(),
                             [&](const ParameterScene::Modifier& m) {
                                return m.destParamCoord == paramCoord;
                             });
   if (modIt == it->modifiers.end())
   {
      it->modifiers.emplace_back(paramCoord, value);
      emitModifierEndValueChanged(sceneUuid, paramCoord, value);
   }
   else
   {
      if (modIt->goalValue != value)
      {
         modIt->goalValue = value;
         emitModifierEndValueChanged(sceneUuid, paramCoord, value);
      }
   }
}

void ParameterSceneContainer::removeModifier(
    const util::Identifiable::UUID& sceneUuid,
    const ParameterCoordinate& paramCoord) noexcept
{
   auto it = std::find_if(
       m_data.begin(), m_data.end(),
       [sceneUuid](const ParameterScene& ps) { return ps.id() == sceneUuid; });
   if (it == m_data.end())
   {
      spdlog::error("No scene found with uuid: {}", util::uuid2Str(sceneUuid));
      return;
   }
   auto modIt = std::find_if(it->modifiers.begin(), it->modifiers.end(),
                             [&](const ParameterScene::Modifier& m) {
                                return m.destParamCoord == paramCoord;
                             });
   if(modIt != it->modifiers.end())
   {
      it->modifiers.erase(modIt);
      emitModifierRemoved(sceneUuid, paramCoord);
   }
}
