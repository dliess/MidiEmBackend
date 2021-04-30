#ifndef INSTRUMENTS_RPC_H
#define INSTRUMENTS_RPC_H

#include "MidiEmInstrumentsRpcIf.h"

namespace base::instruments { struct Instruments; }

namespace uiadapter::capnzero {

class InstrumentsRpc : public ::capnzero::MidiEm::InstrumentsRpcIf {
public:
	InstrumentsRpc(base::instruments::Instruments& rInstruments);
	void triggerSendData() override;
	void createKitInstrument(const ::capnzero::TextView& name) override;
	ReturnRemoveKitInstrument removeKitInstrument(const ::capnzero::SpanCL<8>& instrumentUUID) override;
private:
	base::instruments::Instruments& m_rInstruments;
};

} // namespace uiadapter::capnzero

#endif // INSTRUMENTS_RPC_H