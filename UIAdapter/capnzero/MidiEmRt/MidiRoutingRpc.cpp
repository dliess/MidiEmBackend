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

void MidiRoutingRpc::toggleSpecializedRouting(const ::capnzero::TextView& from,
                                              const ::capnzero::TextView& to,
                                              ::capnzero::Int16 sourceChannel,
                                              ::capnzero::Int16 destChannel)
{
   m_rMidiRouter.toggleMappingForChannelIdx(std::string(from), std::string(to),
                                            sourceChannel, destChannel);
}
