#include "InstrumentsRpc.h"
#include "Instruments.h"

using namespace uiadapter::capnzero;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView &name) {}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<8> &instrumentUUID) {}

void InstrumentsRpc::melodicNoteOn(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    //LOG_F(INFO, "melodicNoteOn {} {} {}", instrumentIndex, note, velocity);
    m_rInstruments.data.melodicInstruments[instrumentIndex]->noteOn(note, velocity);
}

void InstrumentsRpc::melodicNoteOff(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    m_rInstruments.data.melodicInstruments[instrumentIndex]->noteOff(note, velocity);
}

void InstrumentsRpc::kitNoteOn(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    m_rInstruments.data.kitInstruments[instrumentIndex]->noteOn(soundIndex, note, velocity);
}

void InstrumentsRpc::kitNoteOff(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    m_rInstruments.data.kitInstruments[instrumentIndex]->noteOff(soundIndex, note, velocity);
}

