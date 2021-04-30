#include "InstrumentsRpc.h"
#include "Instruments.h"

using namespace uiadapter::capnzero;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView &name) {}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<8> &instrumentUUID) {}
