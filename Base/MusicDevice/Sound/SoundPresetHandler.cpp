#include "SoundPresetHandler.h"

#include "SoundParameterStorage.h"
#include "SoundSection.h"

using namespace base::musicDevice::sound::preset;

PresetHandler::PresetHandler(
    const description::sound::Section& rSoundSection,
    ParameterStorage& rParameterStorage,
    std::shared_ptr<DevicePresets> pSoundPresets) noexcept :
    m_rSoundSection(rSoundSection),
    m_rParameterStorage(rParameterStorage),
    m_pDevicePresets(std::move(pSoundPresets))
{
}

std::vector<std::vector<std::string>> PresetHandler::getSoundPresetList()
    const noexcept
{
   return m_pDevicePresets->getSoundPresetList();
}

std::optional<std::string> PresetHandler::getActualSoundPresetName(
    int voiceIdx) const noexcept
{
   return m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
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
       m_pDevicePresets->preset(engineIdx, *actualPresetName);
   m_rParameterStorage.forEachParameter(
       [&presetData](int paramIdx, ParameterStorage::Element& param) {
          const auto& from = presetData.parameters[paramIdx];
          param.setCommandedValue(from.commanded);
          param.lfo.setAmplitude(from.lfoData.amplitude);
          param.lfo.setFrequency(from.lfoData.frequency);
          param.lfo.setWaveform(from.lfoData.waveform);
          param.lfo.setMultiplierExp(from.lfoData.multiplierExp);
       },
       voiceIdx);
}

void PresetHandler::selectSoundPreset(int voiceIdx,
                                      const std::string& presetName) noexcept
{
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);

   if (!m_pDevicePresets->hasPreset(engineIdx, presetName))
      return;
   m_rParameterStorage.setActualPresetOfVoice(voiceIdx, presetName);
   resetToActualSoundPreset(voiceIdx);
}

void PresetHandler::storeAsSoundPreset(int voiceIdx,
                                       const std::string& presetName,
                                       Category category, Genre genre) noexcept
{
   std::string actPreset;
   if (!presetName.empty())
   {
      actPreset = presetName;
   }
   if (actPreset.empty())
   {
      actPreset = "preset";
   }

   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   actPreset = m_pDevicePresets->incrementNameIdx(engineIdx, actPreset);

   Preset presetData;
   presetData.category = category;
   presetData.genre = genre;
   presetData.parameters.resize(m_rParameterStorage.paramCount(voiceIdx));
   m_rParameterStorage.forEachParameter(
       [&presetData](int paramIdx, const ParameterStorage::Element& param) {
          auto& dest                 = presetData.parameters[paramIdx];
          dest.commanded             = param.commanded;
          dest.lfoData.amplitude     = param.lfo.amplitude();
          dest.lfoData.frequency     = param.lfo.frequency();
          dest.lfoData.waveform      = param.lfo.waveform();
          dest.lfoData.multiplierExp = param.lfo.multiplierExp();
       },
       voiceIdx);

   m_pDevicePresets->savePreset(engineIdx, actPreset, std::move(presetData));
   for(auto& cb : m_changedCbs) cb(engineIdx, presetName);
}

void PresetHandler::stageCurrentState(int voiceIdx) noexcept
{
   // TODO
}

void PresetHandler::resetToStaged(int voiceIdx) noexcept
{
   // TODO
}

void PresetHandler::deletePreset(int voiceIdx,
                                 const std::string& newSelectedPreset) noexcept
{
   const auto actualPreset =
       m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
   if (!actualPreset)
   {
      return;
   }
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   m_pDevicePresets->deletePreset(
       engineIdx, *actualPreset);
   selectSoundPreset(voiceIdx, newSelectedPreset);
   for(auto& cb : m_changedCbs) cb(engineIdx, *actualPreset);
}

void PresetHandler::registerChangedCb(ChangedCb cb) noexcept
{
   m_changedCbs.emplace_back(cb);
}

