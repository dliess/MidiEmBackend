#include "SoundPresetHandler.h"

#include "SoundParameterStorage.h"
#include "SoundSection.h"
#include "spdlog/spdlog.h"

using namespace base::musicDevice::sound::preset;

PresetHandler::PresetHandler(
    const description::sound::Section& rSoundSection,
    ParameterStorage& rParameterStorage,
    std::shared_ptr<DevicePresets> pSoundPresets,
    std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept :
    m_rSoundSection(rSoundSection),
    m_rParameterStorage(rParameterStorage),
    m_pDevicePresets(std::move(pSoundPresets)),
    m_pInitialActualPresetNames(std::move(pActualPresetNames))
{
}

void PresetHandler::initMidiInHandler(sound::MidiInMsgHandlerT* pMidiInMsgHandler)
{
   m_pMidiInMsgHandler = pMidiInMsgHandler;
   if(m_pMidiInMsgHandler)
   {
      m_pMidiInMsgHandler->onProgramChange([this](int voiceIdx, int presetIdx){
         const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
         auto presetName = m_pDevicePresets->getPresetNameByPresetSlot(engineIdx, presetIdx);
         if(presetName)
         {
            selectSoundPreset(voiceIdx, *presetName);
         }
      });
   }
}

void PresetHandler::initMidiOutHandler(
    sound::MidiOutMsgHandlerT* pMidiOutMsgHandler)
{
   m_pMidiOutMsgHandler = pMidiOutMsgHandler;
}

std::optional<std::string> PresetHandler::getActualSoundPresetName(
    int voiceIdx) const noexcept
{
   return m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
}

void PresetHandler::resetToInitialActualSoundPresetsIfSet() noexcept
{
   if(!m_pInitialActualPresetNames) return;
   for(int i = 0; i < m_pInitialActualPresetNames->size(); ++i)
   {
      if(!m_pInitialActualPresetNames->at(i).empty())
      {
         selectSoundPreset(i-1, m_pInitialActualPresetNames->at(i));
      }
   }
   m_pInitialActualPresetNames.reset();
}

void PresetHandler::resetToActualSoundPreset(int voiceIdx) noexcept
{
   const auto& actualPresetName =
       m_rParameterStorage.getActualPresetOfVoice(voiceIdx);
   if (!actualPresetName || actualPresetName->size() == 0)
   {
      return;
   }
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   const auto& presetData =
       m_pDevicePresets->preset(engineIdx, *actualPresetName);
   m_rParameterStorage.forEachParameter(
       [this, &presetData, voiceIdx](int paramIdx, ParameterStorageElement& param) {
          const auto& from = presetData.parameters[paramIdx];
          param.setCommandedValue(from.commanded,
                                  !presetData.slotOnDeviceIndex.has_value());
          param.setActualValueUnsynced(from.commanded);
          // we have to call them by ParameterStorage so cb_signals get emitted on change 
          m_rParameterStorage.setAmplitude(voiceIdx, paramIdx, from.lfoData.amplitude);
          m_rParameterStorage.setFrequency(voiceIdx, paramIdx, from.lfoData.frequency);
          m_rParameterStorage.setWaveform(voiceIdx, paramIdx, from.lfoData.waveform);
          m_rParameterStorage.setMultiplierExp(voiceIdx, paramIdx, from.lfoData.multiplierExp);
       },
       voiceIdx);
   if (m_pMidiOutMsgHandler && presetData.slotOnDeviceIndex.has_value())
   {
      m_pMidiOutMsgHandler->programChange(voiceIdx,
                                          *presetData.slotOnDeviceIndex);
   }
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
                                       const std::string& _presetName,
                                       Category category, Genre genre) noexcept
{
   const std::string presetName = _presetName.empty() ? "preset" : _presetName;
   const auto engineIdx = m_rSoundSection.voice2EngineIdx(voiceIdx);
   //presetName = m_pDevicePresets->incrementNameIdx(engineIdx, presetName);

   Preset presetData;
   presetData.category = category;
   presetData.genre    = genre;
   presetData.parameters.resize(m_rParameterStorage.paramCount(voiceIdx));
   m_rParameterStorage.forEachParameter(
       [&presetData](int paramIdx, const ParameterStorageElement& param) {
          auto& dest                 = presetData.parameters[paramIdx];
          dest.commanded             = param.commanded;
          dest.lfoData.amplitude     = param.lfo.amplitude();
          dest.lfoData.frequency     = param.lfo.frequency();
          dest.lfoData.waveform      = param.lfo.waveform();
          dest.lfoData.multiplierExp = param.lfo.multiplierExp();
       },
       voiceIdx);

   m_pDevicePresets->savePreset(engineIdx, presetName, std::move(presetData));
   emitPresetChanged(engineIdx, presetName);
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
   m_pDevicePresets->deletePreset(engineIdx, *actualPreset);
   selectSoundPreset(voiceIdx, newSelectedPreset);
   emitPresetChanged(engineIdx, *actualPreset);
}