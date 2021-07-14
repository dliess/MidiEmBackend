#include "MidiRoutingRpc.h"

#include "MidiRouter.h"

using namespace uiadapter::capnzero;

MidiRoutingRpc::MidiRoutingRpc(base::midifriends::Router& rMidiRouter) :
    m_rMidiRouter(rMidiRouter)
{
}

void MidiRoutingRpc::toggleRouted(const ::capnzero::TextView& from,
                                  const ::capnzero::TextView& to)
{
   m_rMidiRouter.toggleRouted(std::string(from), std::string(to));
}

void MidiRoutingRpc::toggleSpecializedRoutingEnabled(
    const ::capnzero::TextView& from, const ::capnzero::TextView& to)
{
   m_rMidiRouter.toggleSpecializedRoutingEnabled(std::string(from), std::string(to));
}

void MidiRoutingRpc::initSpecializedRouting(const ::capnzero::TextView& from,
                                            const ::capnzero::TextView& to)
{
   m_rMidiRouter.initSpecialized(std::string(from), std::string(to));
}

void MidiRoutingRpc::clearSpecializedRouting(const ::capnzero::TextView& from,
                                             const ::capnzero::TextView& to)
{
   m_rMidiRouter.clearSpecialized(std::string(from), std::string(to));
}

void MidiRoutingRpc::setSpecializedRouting(const ::capnzero::TextView& from,
                                           const ::capnzero::TextView& to,
                                           ::capnzero::UInt8 sourceChannel,
                                           ::capnzero::UInt8 destChannel,
                                           ::capnzero::UInt8 enable)
{
   m_rMidiRouter.setMappingForChannelIdx(std::string(from), std::string(to),
                                         sourceChannel, destChannel, enable);
}
