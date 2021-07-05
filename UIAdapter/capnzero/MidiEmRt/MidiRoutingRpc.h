#ifndef MIDI_ROUTING_RPC_H
#define MIDI_ROUTING_RPC_H

#include "MidiEmRtMidiRoutingRpcIf.h"

namespace uiadapter::capnzero
{
class MidiRoutingRpc : public ::capnzero::MidiEmRt::MidiRoutingRpcIf
{
public:
	void toggleRouted(const ::capnzero::TextView& from, const ::capnzero::TextView& to) override;
	void initSpecializedRouting(const ::capnzero::TextView& from, const ::capnzero::TextView& to) override;
	void clearSpecializedRouting(const ::capnzero::TextView& from, const ::capnzero::TextView& to) override;
	void setSpecializedRouting(const ::capnzero::TextView& from, const ::capnzero::TextView& to, ::capnzero::UInt8 sourceChannel, const ::capnzero::SpanCL<16>& destMap) override;
private:
};

}   // namespace uiadapter::capnzero
#endif // MIDI_ROUTING_RPC_H