#include "PresetFetcher.h"

#include <thread>

#include "ParameterDumpRequest.h"
#include "SoundMidiInSysExDumpHandler.h"
#include "loguru.hpp"

using namespace base::musicDevice;
using namespace base::musicDevice::sound;

PresetFetcher::PresetFetcher(
    std::shared_ptr<description::Description> pDescription) noexcept :
    m_pDescription(std::move(pDescription))
{
   assert(m_pDescription);
}

void PresetFetcher::addMidiIn(std::shared_ptr<MidiInput> pMidiIn) noexcept
{
   m_pMidiIn = std::move(pMidiIn);
}

void PresetFetcher::addMidiOut(std::shared_ptr<MidiOutput> pMidiOut) noexcept
{
   m_pMidiOut = std::move(pMidiOut);
}

std::shared_ptr<PresetFetcher::MidiInput> PresetFetcher::hijackMidiIn() noexcept
{
   return std::move(m_pMidiIn);
}

std::shared_ptr<PresetFetcher::MidiOutput>
PresetFetcher::hijackMidiOut() noexcept
{
   return std::move(m_pMidiOut);
}

void PresetFetcher::fetchPresets()
{
   // 2 Possible ways:
   // a.) Manual method (this one is implemented here like the Novation Circuit
   // does it)
   // ** capability to set a preset: comes from preset::numPresets TODO: per
   // engine
   // ** capability to dump parameters
   // b.) Dedicated Preset-Dump Method (If device has something)
   //    I DONT IMPLEMENT IT UNTIL I FIND A DEVICE THAT CAN DO IT
   constexpr int INVALID_VOICE_IDX = -2;
   int voiceIdxInFocus             = INVALID_VOICE_IDX;
   m_pMidiIn->registerMidiInCb([this, &voiceIdxInFocus](
                                   const midi::MidiMessage& midiMessage) {
      LOG_F(INFO, "Received {}", midi::toString(midiMessage));
      if (INVALID_VOICE_IDX == voiceIdxInFocus)
      {
         return;
      }
      const auto pSysEX =
          mpark::get_if<midi::Message<midi::SystemExclusive>>(&midiMessage);
      if (pSysEX)
      {
         std::string presetName;
         preset::Preset preset;
         preset.parameters.resize(
             m_pDescription->soundSection->engineBase(voiceIdxInFocus)
                 ->parameters.size());
         // preset.parameters.resize(m_pDescription.);
         const uint8_t TODO_midiVoiceOffset = 0;
         MidiInSysExDumpHandler sysexDumpHandler(
             *m_pDescription->soundSection, TODO_midiVoiceOffset,
             [&preset, &voiceIdxInFocus](int voiceId, int parameterId,
                                         float value) {
                if (voiceIdxInFocus == voiceId)
                {
                   if (util::vector_index_in_range(parameterId,
                                                   preset.parameters))
                   {
                      preset.parameters[parameterId].commanded = value;
                   }
                   else
                   {
                      LOG_F(INFO, "Parameter '{}' not in vector range: 0 .. {}",
                            parameterId, preset.parameters.size());
                   }
                }
             });
         sysexDumpHandler.handle(*pSysEX);
         if (sysexDumpHandler.presetName())
            presetName = sysexDumpHandler.presetName().value();
         if (sysexDumpHandler.presetCategory())
            preset.category = sysexDumpHandler.presetCategory().value();
         if (sysexDumpHandler.presetGenre())
            preset.genre = sysexDumpHandler.presetGenre().value();
         LOG_F(INFO, "Preset '{}' '{}' '{}' received", presetName,
               ~preset.category, ~preset.genre);
         emitPresetReceived(
             m_pDescription->soundSection->voices[voiceIdxInFocus].engineId,
             presetName, std::move(preset));
      }
   });
   m_pDescription->soundSection->forEachEngineBase(
       [this, &voiceIdxInFocus](int engineIdx,
                                description::sound::EngineBase& rEngineBase) {
          if (!rEngineBase.canDumpPresets())
             return;
          bool engineFetched = false;
          for (int voiceIdx = 0;
               voiceIdx < m_pDescription->soundSection->voices.size();
               ++voiceIdx)
          {
             if (engineFetched)
                continue;
             if (m_pDescription->soundSection->voices[voiceIdx].engineId ==
                 engineIdx)
             {
                voiceIdxInFocus = voiceIdx;
                for (int i = 0; i < rEngineBase.presets->numberOfPresets; ++i)
                {
                   // TODO: wrap this
                   m_pMidiOut->send(midi::Message<midi::ProgramChange>(
                       m_pDescription->soundSection->voices[voiceIdx]
                               .midiChannel -
                           1,
                       i));

                   const uint8_t TODO_midiVoiceOffset = 0;
                   ParameterDumpRequest(*m_pMidiOut,
                                        *m_pDescription->soundSection,
                                        TODO_midiVoiceOffset)
                       .sendParameterDumpRequest(voiceIdx);
                   std::this_thread::sleep_for(std::chrono::milliseconds(10));
                   m_pMidiIn->update();
                }
                engineFetched = true;
             }
          }
       });
   m_pMidiIn->clearCbs();
}

bool PresetFetcher::hasMidiInAndOut() const noexcept
{
   return m_pMidiIn && m_pMidiOut;
}
