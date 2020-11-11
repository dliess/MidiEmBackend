#include "SoundPresetHandler.h"

#include "SoundParameterStorage.h"
#include "SoundPresets.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound;

PresetHandler::PresetHandler(
   const description::sound::Section& rSoundSection,
   ParameterStorage& rParameterStorage,
   std::shared_ptr<SoundPresets> pSoundPresets) noexcept :
   m_rSoundSection(rSoundSection),
   m_rParameterStorage(rParameterStorage),
   m_pSoundPresets(std::move(pSoundPresets))
{
}

std::vector<std::vector<std::string>>
PresetHandler::getSoundPresetList() const noexcept
{
   return m_pSoundPresets->getSoundPresetList();
}

std::optional<std::string> PresetHandler::getActualSoundPresetName(
   int voiceIdx) const noexcept
{
   return m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
}

void PresetHandler::registerPresetListChangeCb(
   std::function<void()> cb) noexcept
{
   m_pSoundPresets->registerPresetListChangeCb(cb);
}

void PresetHandler::clearPresetListChangeCb() noexcept
{
   m_pSoundPresets->clearPresetListChangeCb();
}

void PresetHandler::resetToActualSoundPreset(int voiceIdx) noexcept
{
   const auto& actualPresetName =
      m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
   if (!actualPresetName)
   {
      return;
   }
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   const auto& presetData =
      m_pSoundPresets->preset(engineIdx, *actualPresetName);
   m_rParameterStorage.setParameterOfVoice(voiceIdx, presetData);
}

void PresetHandler::selectSoundPreset(
   int voiceIdx, const std::string& presetName) noexcept
{
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);

   if (!m_pSoundPresets->hasSoundPreset(engineIdx, presetName))
      return;
   m_rParameterStorage.setActualPresetOfVoice(voiceIdx, presetName);
   resetToActualSoundPreset(voiceIdx);
}

void PresetHandler::storeAsSoundPreset(
   int voiceIdx, const std::string& presetName) noexcept
{
   std::string actPreset;
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);

   if (!presetName.empty())
   {
      actPreset = presetName;
   }
   else
   {
      if (actPreset.empty())
      {
         actPreset = "preset";
      }
      actPreset = m_pSoundPresets->incrementNameIdx(engineIdx, actPreset);
   }
   m_pSoundPresets->saveSoundPreset(
      engineIdx, actPreset,
      m_rParameterStorage.getCommandedValuesOfVoice(voiceIdx));
}

void PresetHandler::resetParametersToZeroState(int voiceIdx) noexcept
{
   m_rParameterStorage.resetToInitialValues(voiceIdx, m_rSoundSection);
}

void PresetHandler::deleteSoundPreset(
   int voiceIdx, const std::string& newSelectedPreset) noexcept
{
   const auto actualPreset =
      m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
   if (!actualPreset)
   {
      return;
   }
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   m_pSoundPresets->deleteSoundPreset(
      engineIdx, *m_rParameterStorage.getActualPresetOfVoice(voiceIdx));
   selectSoundPreset(voiceIdx, newSelectedPreset);
}
