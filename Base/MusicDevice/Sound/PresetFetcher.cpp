#include "PresetFetcher.h"

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
    if(m_pMidiOut)
    {
        fetchPresets();
    }
}

void PresetFetcher::addMidiOut(std::shared_ptr<MidiOutput> pMidiOut) noexcept
{
    m_pMidiOut = std::move(pMidiOut);
    if(m_pMidiIn)
    {
        fetchPresets();
    }
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
    /*
    if(m_pDescription->.presets && m_rSoundSection.parameterDump)
    {
        m_pMidiIn->registerMidiInCb([](){
            if(m_rSoundSection.parameterDump)
            {
                MidiInSysExDumpHandler sysexDumpHandler(...);
            }
        });
        for(int i = 0; i < m_rSoundSection.presets->numberOfPresets; ++i)
        {
            m_pMidiOut->send(midi::MidiMessage<midi::ProgramChange>(i));
            usleep(10000);
            m_pMidiIn->update();
        }

        m_pMidiIn->clearCbs();
    }
    */
}

bool PresetFetcher::hasMidiInAndOut() const noexcept
{
    return m_pMidiIn && m_pMidiOut;
}
