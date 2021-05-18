#include "MusicDevicesRpc.h"
#include "MusicDeviceContainer.h"

using namespace uiadapter::capnzero;

MusicDevicesRpc::MusicDevicesRpc(base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void MusicDevicesRpc::noteOn(::capnzero::UInt8 mdIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    //m_rMusicDeviceContainer . noteOn();
}

void MusicDevicesRpc::noteOff(::capnzero::UInt8 mdIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{

}