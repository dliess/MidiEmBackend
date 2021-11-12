#include "MainRpc.h"

using namespace uiadapter::capnzero;

MainRpc::MainRpc(RtServer::Signals &rSignals,
                 base::instruments::Instruments &rInstruments,
                 base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer,
                 base::musicDevice::TransportControl &rTransportControl,
                 base::midifriends::Router &rMidiRouter) :
    m_rSignals(rSignals),
    m_rInstruments(rInstruments),
    m_rMusicDeviceContainer(rMusicDeviceContainer),
    m_rTransportControl(rTransportControl),
    m_rMidiRouter(rMidiRouter)
{
}

void MainRpc::reEmitSignals()
{
    LOG_F(INFO, "MainRpc::reEmitSignals() called"); 
    for (auto &it : m_rMusicDeviceContainer)
    {
        const auto uuid        = it.second.get()->id();
        const auto &deviceName = it.second.get()->deviceId().deviceName;
        const auto &portName   = it.second.get()->deviceId().portName;
        const auto mediumId    = it.second.get()->mediumId();
        assert(mediumId.has_value());
        const auto midiVoiceOffset =
            it.second.get()->soundHandler
                ? it.second.get()->soundHandler->getMidiVoiceOffset()
                : 0;
        const base::musicDevice::description::Description &description =
            *it.second.get()->description();
        m_rSignals.MusicDevices__deviceAdded(uuid, deviceName, portName,
                                        mediumId->toStr(),
                                        midiVoiceOffset);
    }
    m_rSignals.Tempo__beatTickStartedChanged(
        base::tempo::BeatTick::instance().running());
    m_rSignals.Tempo__bpmCentsChanged(
        base::tempo::BeatTick::instance().getBpmCents());
    m_rSignals.Instruments__kitInstrumentsChanged(
        meta::serialize(m_rInstruments.data.kitInstruments).dump().c_str());
    m_rSignals.Instruments__melodicInstrumentsChanged(
        meta::serialize(m_rInstruments.data.melodicInstruments)
            .dump()
            .c_str());
    m_rTransportControl.retriggerCallbacks();
    m_rMidiRouter.retriggerCallbacks();
}
