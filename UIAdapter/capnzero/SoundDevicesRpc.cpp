#include "SoundDevicesRpc.h"
#include "MusicDeviceContainer.h"

using namespace uiadapter::capnzero;

SoundDevicesRpc::SoundDevicesRpc(base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void SoundDevicesRpc::registerForParameterChange(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->uiShowsInterestInParameter(voiceIdx, paramIdx);
    }
}

void SoundDevicesRpc::unregisterForParameterChange(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->uiLoosesInterestInParameter(voiceIdx, paramIdx);
    }
}