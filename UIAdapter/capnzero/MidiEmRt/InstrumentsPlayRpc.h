#ifndef RT_INSTRUMENTS_PLAY_RPC_H
#define RT_INSTRUMENTS_PLAY_RPC_H

#include "MidiEmRtInstrumentsPlayRpcIf.h"

namespace base::instruments
{
struct Instruments;
}

namespace uiadapter::capnzero
{
class InstrumentsPlayRpc : public ::capnzero::MidiEmRt::InstrumentsPlayRpcIf
{
public:
   InstrumentsPlayRpc(base::instruments::Instruments& rInstruments);

   void melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                      ::capnzero::Int16 note,
                      ::capnzero::Float32 velocity) override;
   void melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 note,
                       ::capnzero::Float32 velocity) override;
   void kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                  ::capnzero::Int16 soundIndex, ::capnzero::Int16 note,
                  ::capnzero::Float32 velocity) override;
   void kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int16 soundIndex, ::capnzero::Int16 note,
                   ::capnzero::Float32 velocity) override;
   void incrementKitParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 voiceIdx,
                                   ::capnzero::Int16 componentIdx,
                                   ::capnzero::Int16 parameterIdx,
                                   ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
                                   ::capnzero::Float32 increment) override;
   void setKitParameterValue(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceIdx,
                             ::capnzero::Int16 componentIdx,
                             ::capnzero::Int16 parameterIdx,
                             ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
                             ::capnzero::Float32 value) override;
   void blancKitParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 voiceIdx,
                                ::capnzero::Int16 componentIdx) override;
   void incrementMelodicParameterValue(
       const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 componentIdx,
       ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
       ::capnzero::Float32 increment) override;
   void setMelodicParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::Int16 componentIdx,
                                 ::capnzero::Int16 parameterIdx,
                                 ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
                                 ::capnzero::Float32 value) override;
   void blancMelodicParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int16 componentIdx) override;

private:
   base::instruments::Instruments& m_rInstruments;
};

}   // namespace uiadapter::capnzero

#endif   // INSTRUMENTS_RPC_H