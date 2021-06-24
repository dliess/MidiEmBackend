#include "SoundDevicesRpc.h"

#include "MusicDeviceContainer.h"

using namespace uiadapter::capnzero;

SoundDevicesRpc::SoundDevicesRpc(
    base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void SoundDevicesRpc::noteOn(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::UInt8 voiceIndex,
                             ::capnzero::UInt8 note,
                             ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->noteOn(voiceIndex, note, velocity);
   }
}

void SoundDevicesRpc::noteOff(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::UInt8 voiceIndex,
                              ::capnzero::UInt8 note,
                              ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->noteOff(voiceIndex, note, velocity);
   }
}

void SoundDevicesRpc::pitchBend(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::UInt8 voiceIndex,
                                ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->pitchBend(voiceIndex, value);
   }
}

void SoundDevicesRpc::afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::UInt8 voiceIndex,
                                     ::capnzero::UInt8 note,
                                     ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->afterTouchPoly(voiceIndex, note, value);
   }
}

void SoundDevicesRpc::afterTouch(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::UInt8 voiceIndex,
                                 ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->afterTouch(voiceIndex, value);
   }
}

void SoundDevicesRpc::registerForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->uiShowsInterestInParameter(voiceIdx,
                                                             paramIdx);
   }
}

void SoundDevicesRpc::unregisterForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->uiLoosesInterestInParameter(voiceIdx,
                                                              paramIdx);
   }
}

void SoundDevicesRpc::incrementParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceId,
    ::capnzero::Int16 paramIdx, ::capnzero::Float32 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incrementParameterValue(voiceId, paramIdx,
                                                          increment);
   }
}

void SoundDevicesRpc::setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                        ::capnzero::Int16 voiceId,
                                        ::capnzero::Int16 paramIdx,
                                        ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setParameterValue(voiceId, paramIdx, value);
   }
}