#include "PresetFetcher.h"
#include <thread>

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
    if(m_pDescription->soundSection->parameterDumpAnswer)
    {
        m_pMidiIn->registerMidiInCb([this](const midi::MidiMessage& midiMessage){
            if(m_pDescription->soundSection->parameterDumpAnswer)
            {
                //MidiInSysExDumpHandler sysexDumpHandler(...);
            }
        });
        for(int i = 0; i < m_pDescription->soundSection->presets->numberOfPresets; ++i)
        {
            m_pMidiOut->send(midi::Message<midi::ProgramChange>(0, i));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            m_pMidiIn->update();
        }

        m_pMidiIn->clearCbs();
    }
}

bool PresetFetcher::hasMidiInAndOut() const noexcept
{
    return m_pMidiIn && m_pMidiOut;
}
