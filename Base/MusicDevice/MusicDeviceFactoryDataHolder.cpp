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
   const auto uuidIt = m_musicDevices.find(uuid);
   if (uuidIt == m_musicDevices.end())
   {
      return;
   }
   const auto it = m_actualPresetNames.find(uuidIt->second->deviceId());
   if (it == m_actualPresetNames.end())
   {
      return;
   }
   it->second->at(voiceIdx + 1) = newPresetName;
   util::Settings settings("EnginePresets", "ActualPresets.json");
   settings.save(uuidIt->second->deviceId().toStr(), *it->second.get());
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
   return pDescr;
}

const description::Description* factory::DataHolder::getDescription(
    util::Identifiable::UUIDView uuid) const noexcept
{
   const auto pMdId = getMdIdByUUID(uuid);
   if (!pMdId)
      return nullptr;
   const auto itDescr = m_descriptionCache.find(pMdId->deviceName());
   if (itDescr == m_descriptionCache.end())
      return nullptr;
   return itDescr->second.get();
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
      auto descrPtr = getDescription(id.deviceName());
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
            spdlog::warn(
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
   /* TODO: for now, not here, but from rt-thread because of race confdition of
   signals for(const auto& entries : m_musicDevices)
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

std::optional<MusicDeviceId> factory::DataHolder::getMdIdByUUID(
    util::Identifiable::UUIDView uuid) const noexcept
{
   const auto it = m_musicDevices.find(util::deepCopy(uuid));
   if (it == m_musicDevices.end())
   {
      return std::nullopt;
   }
   return it->second->deviceId();
}

std::optional<util::Identifiable::UUID> factory::DataHolder::getUUIDByMdId(
    const MusicDeviceId& mdId) const noexcept
{
   const auto it = std::find_if(
       m_musicDevices.begin(), m_musicDevices.end(),
       [&mdId](const auto& e) { return e.second->deviceId() == mdId; });
   if (it == m_musicDevices.end())
   {
      const auto it2 = std::find_if(
          m_musicDevices.begin(), m_musicDevices.end(), [&mdId](const auto& e) {
             return e.second->deviceId() ==
                    MusicDeviceId{mdId.deviceName(), MusicDeviceId::ANY_PORT};
          });
      if (it2 == m_musicDevices.end())
      {
         return std::nullopt;
      }
      return it2->first;
   }
   return it->first;
}

MusicDevice* factory::DataHolder::getMusicDeviceByUUID(
    util::Identifiable::UUIDView uuid) const noexcept
{
   const auto it = m_musicDevices.find(util::deepCopy(uuid));
   if (it == m_musicDevices.end())
   {
      return nullptr;
   }
   return it->second;
}

void factory::DataHolder::addUuid2MdId(const util::Identifiable::UUID& uuid,
                                       MusicDevice* md) noexcept
{
   auto [iter, success] = m_musicDevices.emplace(uuid, md);
   if (success)
   {
      emitMusicDeviceAdded(iter->second);
   }
}

void factory::DataHolder::removeEntryForUuid(
    const util::Identifiable::UUID& uuid) noexcept
{
   auto md = getMusicDeviceByUUID(uuid);
   if(md)
   {
      emitMusicDeviceAboutToRemove(md);
      m_musicDevices.erase(uuid);
   }
}