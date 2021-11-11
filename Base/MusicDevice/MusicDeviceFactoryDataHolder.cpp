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
   auto it = presetCache.find(enginePresetId.musicDeviceName);
   if (it != presetCache.end())
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

std::shared_ptr<description::Description> factory::DataHolder::getDescription(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<description::Description> pDescr;
   auto itDescr = descriptionCache.find(deviceName);
   if (itDescr != descriptionCache.end())
   {
      pDescr = itDescr->second;
   }
   else
   {
      pDescr =
          description::loadDescription(m_configDir, deviceName);   // can throw
      pDescr->checkValidity();                                     // can throw
      pDescr->initCaches();
      descriptionCache[deviceName] = pDescr;
   }
   return std::move(pDescr);
}

std::shared_ptr<sound::preset::DevicePresets>
factory::DataHolder::getDevicePresets(
    const MusicDeviceName& deviceName) noexcept
{
   std::shared_ptr<sound::preset::DevicePresets> pPresets;
   auto it = presetCache.find(deviceName);
   if (it != presetCache.end())
   {
      pPresets = it->second;
   }
   else
   {   // TODO: only if it has sound section and sound parameters
       // if (pDescr->soundSection && pDescr->soundSection->hasParameters()) {
      // pPresets = m_loader.load(deviceName);   // can throw
      pPresets = std::make_shared<sound::preset::DevicePresets>(deviceName);
      pPresets->load();
      pPresets->forEachPreset([this, &deviceName](
                                  int engineIdx, const std::string& presetName,
                                  const sound::preset::Preset& preset) {
         emitPresetUpdated(
             sound::preset::Id({deviceName, engineIdx, presetName}),
             preset.category, preset.genre);
      });
      presetCache[deviceName] = pPresets;
      //}
   }
   return std::move(pPresets);
}
