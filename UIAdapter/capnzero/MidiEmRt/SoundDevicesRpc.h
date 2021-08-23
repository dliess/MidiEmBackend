#ifndef SOUND_DEVICES_RPC_H
#define SOUND_DEVICES_RPC_H

#include "MidiEmRtSoundDevicesRpcIf.h"

namespace base::musicDevice
{
class MusicDeviceContainer;
}

namespace uiadapter::capnzero
{
class SoundDevicesRpc : public ::capnzero::MidiEmRt::SoundDevicesRpcIf
{
public:
   SoundDevicesRpc(
       base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer);
   void noteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 voiceIndex,
               ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
   void noteOff(const ::capnzero::SpanCL<16>& uuid,
                ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note,
                ::capnzero::Float32 velocity) override;
   void pitchBend(const ::capnzero::SpanCL<16>& uuid,
                  ::capnzero::UInt8 voiceIndex,
                  ::capnzero::Float32 value) override;
   void afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::UInt8 voiceIndex, ::capnzero::UInt8 note,
                       ::capnzero::Float32 value) override;
   void afterTouch(const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::UInt8 voiceIndex,
                   ::capnzero::Float32 value) override;

   void registerForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 voiceIdx,
                                   ::capnzero::Int16 paramIdx) override;
   void unregisterForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx,
                                     ::capnzero::Int16 paramIdx) override;
   void incrementParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 voiceId,
                                ::capnzero::Int16 paramIdx,
                                ::capnzero::Float32 increment) override;
   void setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                          ::capnzero::Int16 voiceId, ::capnzero::Int16 paramIdx,
                          ::capnzero::Float32 value) override;
	void blankVoiceParameter(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int16 voiceId, 
                            ::capnzero::Int16 paramIdx) override;
   void blankVoiceParameters(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceId) override;
   void blankAllVoiceParameters(const ::capnzero::SpanCL<16>& uuid) override;

private:
   base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
};

}   // namespace uiadapter::capnzero

#endif   // SOUND_DEVICES_RPC_H