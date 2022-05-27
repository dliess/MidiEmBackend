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
   void noteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
               ::capnzero::Int16 note, ::capnzero::Float32 velocity) override;
   void noteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
                ::capnzero::Int16 note, ::capnzero::Float32 velocity) override;
   void pitchBend(const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
                  ::capnzero::Float32 value) override;
   void afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 note,
                       ::capnzero::Float32 value) override;
   void afterTouch(const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int16 voiceIdx,
                   ::capnzero::Float32 value) override;

   void registerForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 voiceIdx,
                                   ::capnzero::Int16 paramIdx) override;
   void unregisterForParameterChange(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx,
                                     ::capnzero::Int16 paramIdx) override;
   void incrementParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 voiceIdx,
                                ::capnzero::Int16 paramIdx,
                                ::capnzero::Float32 increment) override;
   void setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                          ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                          ::capnzero::Float32 value) override;
   void blankVoiceParameter(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int16 voiceIdx,
                            ::capnzero::Int16 paramIdx) override;
   void blankVoiceParameters(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceIdx) override;
   void blankAllVoiceParameters(const ::capnzero::SpanCL<16>& uuid) override;
   void setLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                       ::capnzero::MidiEmRt::LFOWaveform waveform) override;
   void incLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                       ::capnzero::Int16 increment) override;
   void setLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 amplitude) override;
   void incLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 increment) override;
   void setLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 frequency) override;
   void incLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int16 voiceIdx, ::capnzero::Int16 paramIdx,
                        ::capnzero::Float32 increment) override;
   void setLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int16 voiceIdx,
                            ::capnzero::Int16 paramIdx,
                            ::capnzero::UInt32 exponent) override;
   void incLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int16 voiceIdx,
                            ::capnzero::Int16 paramIdx,
                            ::capnzero::Int32 increment) override;
   void stageParameterValues(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceIdx) override;
   void restoreToStagedParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                       ::capnzero::Int16 voiceIdx) override;
   void restoreToLastActualPreset(const ::capnzero::SpanCL<16>& uuid,
                                  ::capnzero::Int16 voiceIdx) override;
   void saveAsPreset(const ::capnzero::SpanCL<16>& uuid,
                     ::capnzero::Int16 voiceIdx,
                     const ::capnzero::TextView& presetName,
                     const ::capnzero::TextView& category,
                     const ::capnzero::TextView& genre) override;
   void setActualPreset(const ::capnzero::SpanCL<16>& uuid,
                        ::capnzero::Int16 voiceIdx,
                        const ::capnzero::TextView& presetName) override;
   void sendParameterDumpRequest(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::Int16 voiceIdx) override;

   void setArpeggiatorBypass(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceIdx,
                             ::capnzero::Int16 on) override;
   void setArpeggiatorStepLength(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::Int16 voiceIdx,
                                 ::capnzero::Float32 stepLength) override;
   void setArpeggiatorGateFill(const ::capnzero::SpanCL<16>& uuid,
                               ::capnzero::Int16 voiceIdx,
                               ::capnzero::Float32 gateFill) override;
   void setArpeggiatorAlgorithm(
       const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::MidiEmRt::ArpeggiatorAlgorithm algorithm) override;
   void setArpeggiatorRange(
       const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::MidiEmRt::ArpeggiatorRangeType rangeType,
       ::capnzero::Int32 range) override;
   void setArpeggiatorHoldNotes(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 voiceIdx,
                                ::capnzero::Int16 on) override;
   void setArpeggiatorFeedMode(
       const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::MidiEmRt::ArpeggiatorFeedMode feedMode) override;
   void arpeggiatorSeqInsertPause(const ::capnzero::SpanCL<16>& uuid,
                                  ::capnzero::Int16 voiceIdx) override;
   void arpeggiatorSeqRemoveLastNote(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx) override;

private:
   base::musicDevice::MusicDeviceContainer& m_rMusicDeviceContainer;
   template<typename Cb>
   void withSoundHandlerDo(const ::capnzero::SpanCL<16>& uuid, Cb&& cb);
};

}   // namespace uiadapter::capnzero

#include "SoundDevicesRpc.inl"

#endif   // SOUND_DEVICES_RPC_H