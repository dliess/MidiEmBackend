#include "InstrumentsRpc.h"

#include "Instruments.h"

using namespace uiadapter::capnzero;
using namespace base::instruments;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{
}

void InstrumentsRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 note,
                                   ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrument(uuid, [&](const MelodicInstrument& mi){
      mi.noteOn(note, velocity);
   });
}

void InstrumentsRpc::melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int16 note,
                                    ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrument(uuid, [&](const MelodicInstrument& mi){
      mi.noteOff(note, velocity);
   });
}

void InstrumentsRpc::kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                               ::capnzero::Int16 soundIndex,
                               ::capnzero::Int16 note,
                               ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrument(uuid, [&](const KitInstrument& ki){
      ki.noteOn(soundIndex, note, velocity);
   });
}

void InstrumentsRpc::kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 soundIndex,
                                ::capnzero::Int16 note,
                                ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrument(uuid, [&](const KitInstrument& ki){
      ki.noteOff(soundIndex, note, velocity);
   });
}
