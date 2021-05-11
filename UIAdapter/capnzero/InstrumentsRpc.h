#ifndef INSTRUMENTS_RPC_H
#define INSTRUMENTS_RPC_H

#include "MidiEmInstrumentsRpcIf.h"

namespace base::instruments { struct Instruments; }

namespace uiadapter::capnzero {

class InstrumentsRpc : public ::capnzero::MidiEm::InstrumentsRpcIf {
public:
	InstrumentsRpc(base::instruments::Instruments& rInstruments);
	void createKitInstrument(const ::capnzero::TextView& name) override;
	void removeKitInstrument(const ::capnzero::SpanCL<8>& instrumentUUID) override;
	void melodicNoteOn(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
	void melodicNoteOff(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
	void kitNoteOn(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;
	void kitNoteOff(::capnzero::UInt8 instrumentIndex, ::capnzero::UInt8 soundIndex, ::capnzero::UInt8 note, ::capnzero::Float32 velocity) override;

private:
	base::instruments::Instruments& m_rInstruments;
};

} // namespace uiadapter::capnzero

#endif // INSTRUMENTS_RPC_H