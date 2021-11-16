#include "PresetFetcher.h"

#include <thread>

#include "SoundMidiInSysExDumpHandler.h"

using namespace base::musicDevice;
using namespace base::musicDevice::sound;

PresetFetcher::PresetFetcher(
    MusicDeviceId musicDeviceId,
    std::shared_ptr<description::Description> pDescription) noexcept :
    m_pDescription(std::move(pDescription))
{
   assert(m_pDescription);
}

const MusicDeviceId& PresetFetcher::musicDeviceId() const noexcept
{
   return m_musicDeviceId;
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
    // a.) Manual method (this one is impolemented here like the Novation Circuit does it)
    // ** capability to set a preset: comes from preset::numPresets TODO: per engine
    // ** capability to dump parameters
    // b.) Dedicated Preset-Dump Method (If device has something) 
    //    I DONT IMPLEMENT IT UNTIL I FIND A DEVICE THAT CAN DO IT
    /* TDOD
   if (!m_pDescription->soundSection->parameterDumpAnswer)
   {
      return;
   }
   */
   m_pMidiIn->registerMidiInCb([this](const midi::MidiMessage& midiMessage) {
      const auto pSysEX =
          mpark::get_if<midi::Message<midi::SystemExclusive>>(&midiMessage);
      if (pSysEX)
      {
         preset::Preset preset;
         //preset.parameters.resize(m_pDescription.);
         const uint8_t TODO_midiVoiceOffset = 0;
         MidiInSysExDumpHandler sysexDumpHandler(
             *m_pDescription->soundSection, TODO_midiVoiceOffset,
             [&preset](int voiceId, int parameterId, float value) {
                 preset.parameters.push_back({  });
             });
         sysexDumpHandler.handle(*pSysEX);
      }
   });
   for(int engineIdx = 0; engineIdx < m_pDescription->soundSection->engines.size(); ++engineIdx)
   {

   }
   //for (int i = 0; i < m_pDescription->soundSection->presets->numberOfPresets;
   //     ++i)
   {
   //   m_pMidiOut->send(midi::Message<midi::ProgramChange>(0, i));
      // TODO: also send parameter dump request !!!!!!!!!!!!!!!!!!!!!
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      m_pMidiIn->update();
   }

   m_pMidiIn->clearCbs();
}

bool PresetFetcher::hasMidiInAndOut() const noexcept
{
   return m_pMidiIn && m_pMidiOut;
}
