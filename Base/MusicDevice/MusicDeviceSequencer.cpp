#include "MusicDeviceSequencer.h"

using namespace base::musicDevice;

Sequencer::~Sequencer()
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->unregisterStartedChangedCb(this);
        m_midiOutHandler->unregisterTransportMaskChangedCb(this);
    }
}

void Sequencer::initMidiOutHandler(std::shared_ptr<midi::Midi1Output> pMidiOut) noexcept
{
    m_midiOutHandler = std::move(pMidiOut);
    m_midiOutHandler->registerStartedChangedCb(this, [this](bool started){
        for(auto& cb : m_startedChangedCbs) cb(started);
    });
    m_midiOutHandler->registerTransportMaskChangedCb(this, [this](bool masked){
        for(auto& cb : m_transportMaskChangedCbs) cb(masked);
    });
}

void Sequencer::registerStartedChangedCb(StartedChangedCb cb)
{
    m_startedChangedCbs.push_back(cb);
}

void Sequencer::registerTransportMaskChangedCb(TransportMaskChangedCb cb)
{
    m_transportMaskChangedCbs.push_back(cb);
}

void Sequencer::start() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->start();
    }
}

void Sequencer::stop() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->stop();
    }
}

void Sequencer::toggleEnabled() noexcept
{
    if(m_midiOutHandler)
    {
        m_midiOutHandler->setTransportMasked(!m_midiOutHandler->getTransportMasked());
    }
}

