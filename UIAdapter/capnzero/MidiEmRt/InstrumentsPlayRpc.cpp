#include "InstrumentsPlayRpc.h"

#include "Instruments.h"

using namespace uiadapter::capnzero;
using namespace base::instruments;

InstrumentsPlayRpc::InstrumentsPlayRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{
}

void InstrumentsPlayRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 note,
                                   ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrumentRt(uuid, [&](const MelodicInstrument& mi){
      mi.noteOn(note, velocity);
   });
}

void InstrumentsPlayRpc::melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int16 note,
                                    ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrumentRt(uuid, [&](const MelodicInstrument& mi){
      mi.noteOff(note, velocity);
   });
}

void InstrumentsPlayRpc::kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                               ::capnzero::Int16 soundIndex,
                               ::capnzero::Int16 note,
                               ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki){
      ki.noteOn(soundIndex, note, velocity);
   });
}

void InstrumentsPlayRpc::kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 soundIndex,
                                ::capnzero::Int16 note,
                                ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki){
      ki.noteOff(soundIndex, note, velocity);
   });
}
