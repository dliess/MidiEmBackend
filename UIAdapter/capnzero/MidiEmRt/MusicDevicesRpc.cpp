#include "MusicDevicesRpc.h"
#include "MusicDeviceContainer.h"

using namespace uiadapter::capnzero;

MusicDevicesRpc::MusicDevicesRpc(base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void MusicDevicesRpc::noteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->noteOn(voiceIndex, note, velocity);
    }
}

void MusicDevicesRpc::noteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->noteOff(voiceIndex, note, velocity);
    }
}

void MusicDevicesRpc::pitchBend(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::Float32 value)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->pitchBend(voiceIndex, value);
    }
}

void MusicDevicesRpc::afterTouchPoly(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note, ::capnzero::Float32 value)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->afterTouchPoly(voiceIndex, note, value);
    }
}

void MusicDevicesRpc::afterTouch(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex, ::capnzero::Float32 value)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rMusicDeviceContainer.find(uuid_);
    if(iter != m_rMusicDeviceContainer.end())
    {
        assert(iter->second->soundHandler);
        iter->second->soundHandler->afterTouch(voiceIndex, value);
    }
}
