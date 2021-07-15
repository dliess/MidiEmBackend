#ifndef MIDI_ROUTING_RPC_H
#define MIDI_ROUTING_RPC_H

#include "MidiEmRtMidiRoutingRpcIf.h"

namespace base::midifriends
{
class Router;
}

namespace uiadapter::capnzero
{
class MidiRoutingRpc : public ::capnzero::MidiEmRt::MidiRoutingRpcIf
{
public:
   MidiRoutingRpc(base::midifriends::Router& rMidiRouter);
   void toggleRouted(const ::capnzero::TextView& from,
                     const ::capnzero::TextView& to) override;
   void toggleSpecializedRoutingEnabled(
       const ::capnzero::TextView& from,
       const ::capnzero::TextView& to) override;
   void initSpecializedRouting(const ::capnzero::TextView& from,
                               const ::capnzero::TextView& to) override;
   void clearSpecializedRouting(const ::capnzero::TextView& from,
                                const ::capnzero::TextView& to) override;
   void toggleSpecializedRouting(const ::capnzero::TextView& from,
                                 const ::capnzero::TextView& to,
                                 ::capnzero::UInt8 sourceChannel,
                                 ::capnzero::UInt8 destChannel) override;

private:
   base::midifriends::Router& m_rMidiRouter;
};

}   // namespace uiadapter::capnzero
#endif   // MIDI_ROUTING_RPC_H