#include "InstrumentsRpc.h"
#include "Instruments.h"

using namespace uiadapter::capnzero;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView &name) {}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<8> &instrumentUUID) {}

void InstrumentsRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rInstruments.data.melodicInstruments.findByUUID(uuid_);
    if(iter != m_rInstruments.data.melodicInstruments.end())
    {
        iter->get()->noteOn(note, velocity);
        return;
    }
}

void InstrumentsRpc::melodicNoteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rInstruments.data.melodicInstruments.findByUUID(uuid_);
    if(iter != m_rInstruments.data.melodicInstruments.end())
    {
        iter->get()->noteOff(note, velocity);
        return;
    }
}

void InstrumentsRpc::kitNoteOn(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rInstruments.data.kitInstruments.findByUUID(uuid_);
    if(iter != m_rInstruments.data.kitInstruments.end())
    {
        iter->get()->noteOn(soundIndex, note, velocity);
        return;
    }
}

void InstrumentsRpc::kitNoteOff(const ::capnzero::SpanCL<16>& uuid, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity)
{
    util::Identifiable::UUID uuid_;
    std::copy(uuid.begin(), uuid.end(), uuid_.begin());
    auto iter = m_rInstruments.data.kitInstruments.findByUUID(uuid_);
    if(iter != m_rInstruments.data.kitInstruments.end())
    {
        iter->get()->noteOff(soundIndex, note, velocity);
        return;
    }
}

