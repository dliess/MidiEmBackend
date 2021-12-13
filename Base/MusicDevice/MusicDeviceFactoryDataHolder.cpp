#include "MusicDeviceFactoryDataHolder.h"

#include "DeviceDescriptionLoader.h"

using namespace base::musicDevice;

factory::DataHolder::DataHolder(std::string configDir) noexcept :
    m_configDir(std::move(configDir.empty() ? "." : configDir))
{
}

void factory::DataHolder::soundDevicesPresetChanged(
    const sound::preset::Id& enginePresetId)
{
   auto it = m_presetCache.find(enginePresetId.musicDeviceName);
   if (it != m_presetCache.end())
   {
      const auto attrs = it->second->getPresetAttributes(
          enginePresetId.engineIdx, enginePresetId.presetName);
      if (attrs)
      {
         emitPresetUpdated(enginePresetId, attrs->first, attrs->second);
      }
      else
      {
         emitPresetRemoved(enginePresetId);
      }
      it->second->save();
   }
}

void factory::DataHolder::soundDeviceActualPresetNameChanged(
    const util::Identifiable::UUID& uuid, int voiceIdx,
    const std::string& newPresetName) noexcept
{
   const auto uuidIt = m_uuidToDevIdMap.find(uuid);
   if (uuidIt == m_uuidToDevIdMap.end())
   {
      return;
   }
   const auto it = m_actualPresetNames.find(uuidIt->second);
   if (it == m_actualPresetNames.end())
   {
      return;
   }
   it->second->at(voiceIdx + 1) = newPresetName;
   util::Settings settings("EnginePresets", "ActualPresets.json");
   settings.save(uuidIt->second.toStr(), *it->second.get());
   emitActualPresetNameChanged(uuid, voiceIdx, newPresetName);
}

std::shared_ptr<description::Description> factory::DataHolder::getDescription(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<description::Description> pDescr;
   auto itDescr = m_descriptionCache.find(deviceName);
   if (itDescr != m_descriptionCache.end())
   {
      pDescr = itDescr->second;
   }
   else
   {
      pDescr =
          description::loadDescription(m_configDir, deviceName);   // can throw
      pDescr->checkValidity();                                     // can throw
      pDescr->initCaches();
      m_descriptionCache[deviceName] = pDescr;
      emitDescriptionAdded(deviceName, *pDescr);
   }
   return std::move(pDescr);
}

std::shared_ptr<sound::preset::DevicePresets>
factory::DataHolder::getDevicePresets(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<sound::preset::DevicePresets> pPresets;
   auto it = m_presetCache.find(deviceName);
   if (it != m_presetCache.end())
   {
      pPresets = it->second;
   }
   else
   {
      const auto descrIter = m_descriptionCache.find(deviceName);
      if (descrIter == m_descriptionCache.end() ||
          !descrIter->second->soundSection)
      {
         return nullptr;
      }
      pPresets = std::make_shared<sound::preset::DevicePresets>(
          deviceName, descrIter->second->soundSection->engines.size());
      pPresets->load();
      pPresets->forEachPreset(
          [this, &deviceName](int engineIdx, const std::string& presetName,
                              const sound::preset::Preset& preset) {
             emitPresetUpdated(
                 sound::preset::Id({deviceName, engineIdx, presetName}),
                 preset.category, preset.genre);
          });
      m_presetCache[deviceName] = pPresets;
   }
   return std::move(pPresets);
}

std::shared_ptr<factory::DataHolder::ActualPresetNames>
factory::DataHolder::getActualDevicePresetNames(
    const MusicDeviceId& id) noexcept
{
   auto it = m_actualPresetNames.find(id);
   if (it == m_actualPresetNames.end())
   {
      auto descrPtr = getDescription(id.deviceName);
      if (!descrPtr || !descrPtr->soundSection)
      {
         return nullptr;
      }
      const size_t vSize = descrPtr->soundSection->voices.size() + 1;
      // TODO search without port
      util::Settings settings("EnginePresets", "ActualPresets.json");
      try
      {
         const auto loadedPresetNames =
             settings.load<ActualPresetNames>(id.toStr());
         auto sh = std::make_shared<ActualPresetNames>(loadedPresetNames);
         if (sh->size() != vSize)
         {
            LOG_F(
                WARNING,
                "Loaded config voice size mismatch. loaded: {} != expected {}",
                sh->size(), vSize);
            sh->resize(vSize);
         }
         it = m_actualPresetNames.emplace(id, std::move(sh)).first;
      }
      catch (const std::exception& e)
      {
         it = m_actualPresetNames
                  .emplace(id, std::make_shared<ActualPresetNames>(vSize))
                  .first;
      }
   }
   return it->second;
}

void factory::DataHolder::reEmitSignals()
{
   for (const auto& e : m_descriptionCache)
   {
      emitDescriptionAdded(e.first, *e.second);
   }
   for (const auto& e : m_presetCache)
   {
      const std::string deviceName = e.first;
      e.second->forEachPreset(
          [this, &deviceName](int engineIdx, const std::string& presetName,
                              const sound::preset::Preset& preset) {
             emitPresetUpdated(
                 sound::preset::Id({deviceName, engineIdx, presetName}),
                 preset.category, preset.genre);
          });
   }
   /* TODO: for now, not here, but from rt-thread because of race confdition of signals
   for(const auto& entries : m_uuidToDevIdMap)
   {
      const auto it = m_actualPresetNames.find(entries.second);
      if(it != m_actualPresetNames.end())
      {
         for(int i = 0; i < it->second->size(); ++i)
         {
            const auto& presetName = it->second->at(i);
            if(!presetName.empty())
            {
               emitActualPresetNameChanged(entries.first, i - 1, presetName);
            }
         }
      }
   }
   */
}

void factory::DataHolder::addUuid2MdId(const util::Identifiable::UUID& uuid,
                                       const MusicDeviceId& mdId) noexcept
{
   m_uuidToDevIdMap.emplace(uuid, mdId);
}

void factory::DataHolder::removeEntryForUuid(
    const util::Identifiable::UUID& uuid) noexcept
{
   m_uuidToDevIdMap.erase(uuid);
}