#ifndef RT_INSTRUMENTS_RPC_H
#define RT_INSTRUMENTS_RPC_H

#include "MidiEmRtInstrumentsRpcIf.h"

namespace base::instruments
{
struct Instruments;
}

namespace uiadapter::capnzero
{
class InstrumentsRpc : public ::capnzero::MidiEmRt::InstrumentsRpcIf
{
public:
   InstrumentsRpc(base::instruments::Instruments& rInstruments);

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

private:
   base::instruments::Instruments& m_rInstruments;
};

}   // namespace uiadapter::capnzero

#endif   // INSTRUMENTS_RPC_H