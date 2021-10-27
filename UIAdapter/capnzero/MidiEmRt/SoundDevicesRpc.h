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
   void noteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
               ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
   void noteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
                ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
   void pitchBend(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
                  ::capnzero::Float32 value) override;
   void afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int8 voiceIdx, ::capnzero::UInt8 note,
                       ::capnzero::Float32 value) override;
   void afterTouch(const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int8 voiceIdx,
                   ::capnzero::Float32 value) override;

   void registerForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int8 voiceIdx,
                                   ::capnzero::Int16 paramIdx) override;
   void unregisterForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int8 voiceIdx,
                                     ::capnzero::Int16 paramIdx) override;
   void incrementParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int8 voiceIdx,
                                ::capnzero::Int16 paramIdx,
                                ::capnzero::Float32 increment) override;
   void setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                          ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                          ::capnzero::Float32 value) override;
   void blankVoiceParameter(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int8 voiceIdx,
                            ::capnzero::Int16 paramIdx) override;
   void blankVoiceParameters(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int8 voiceIdx) override;
   void blankAllVoiceParameters(const ::capnzero::SpanCL<16>& uuid) override;
   void setLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                       ::capnzero::MidiEmRt::LFOWaveform waveform) override;
   void incLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                       ::capnzero::Int8 increment) override;
   void setLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 amplitude) override;
   void incLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 increment) override;
   void setLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 frequency) override;
   void incLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int8 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 increment) override;
   void setLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int8 voiceIdx,
                            ::capnzero::Int16 paramIdx,
                            ::capnzero::UInt32 exponent) override;
   void incLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int8 voiceIdx,
                            ::capnzero::Int16 paramIdx,
                            ::capnzero::Int32 increment) override;
   void stageParameterValues(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int8 voiceIdx) override;
   void restoreToStagedParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                       ::capnzero::Int8 voiceIdx) override;
   void restoreToLastActualPreset(const ::capnzero::SpanCL<16>& uuid,
                                  ::capnzero::Int8 voiceIdx) override;
   void saveAsPreset(const ::capnzero::SpanCL<16>& uuid,
                     ::capnzero::Int8 voiceIdx,
                     const ::capnzero::TextView& presetName) override;
   void setActualPreset(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int8 voiceIdx,
                        const ::capnzero::TextView& presetName) override;

private:
   base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
};

}   // namespace uiadapter::capnzero

#endif   // SOUND_DEVICES_RPC_H