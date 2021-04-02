#ifndef INSTRUMENTS_RPC_H
#define INSTRUMENTS_RPC_H

#include "MidiEmInstrumentsRpcIf.h"

namespace uiadapter::capnzero {

class InstrumentsRpc : public ::capnzero::MidiEm::InstrumentsRpcIf {
public:
	void triggerSendData() override;
	void createKitInstrument(const ::capnzero::TextView& name) override;
	ReturnRemoveKitInstrument removeKitInstrument(const ::capnzero::SpanCL<8>& instrumentUUID) override;
};

} // namespace uiadapter::capnzero

#endif // INSTRUMENTS_RPC_H