#include "SessionRpc.h"

using namespace uiadapter::capnzero;

void SessionRpc::pushBackTrack(const ::capnzero::TextView& name) {}
void SessionRpc::addTrack(const ::capnzero::TextView& name,
                          ::capnzero::Int16 position)
{
}
void SessionRpc::duplicateTrack(const ::capnzero::SpanCL<16>& trackUuid) {}
void SessionRpc::removeTrack(const ::capnzero::SpanCL<16>& trackUuid) {}
void SessionRpc::renameTrack(const ::capnzero::SpanCL<16>& trackUuid,
                             const ::capnzero::TextView& name)
{
}
void SessionRpc::moveTrack(const ::capnzero::SpanCL<16>& track1Uuid,
                           ::capnzero::Int16 afterPosition)
{
}
void SessionRpc::toggleTrackMute(const ::capnzero::SpanCL<16>& track1Uuid) {}
void SessionRpc::createClip(const ::capnzero::SpanCL<16>& trackUuid,
                            const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::duplicateClip(const ::capnzero::SpanCL<16>& trackUuid,
                               const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::deleteClip(const ::capnzero::SpanCL<16>& trackUuid,
                            const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::renameClip(const ::capnzero::SpanCL<16>& trackUuid,
                            const ::capnzero::SpanCL<16>& slot,
                            const ::capnzero::TextView& name)
{
}
void SessionRpc::addNoteToClip(const ::capnzero::SpanCL<16>& trackUuid,
                               const ::capnzero::SpanCL<16>& slot,
                               ::capnzero::Float32 beat,
                               const ::capnzero::SpanCL<16>& note,
                               ::capnzero::Float32 velocity)
{
}
void SessionRpc::changeNoteVelocity(const ::capnzero::SpanCL<16>& trackUuid,
                                    const ::capnzero::SpanCL<16>& slot,
                                    ::capnzero::Float32 beat,
                                    const ::capnzero::SpanCL<16>& note,
                                    ::capnzero::Float32 velocity)
{
}
void SessionRpc::removeNoteFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                    const ::capnzero::SpanCL<16>& slot,
                                    ::capnzero::Float32 beat,
                                    const ::capnzero::SpanCL<16>& note)
{
}
void SessionRpc::removeAllNotesFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                        const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::moveClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                          const ::capnzero::SpanCL<16>& srcSlot,
                          const ::capnzero::SpanCL<16>& destTrackUuid,
                          const ::capnzero::SpanCL<16>& destSlot)
{
}
void SessionRpc::startClip(const ::capnzero::SpanCL<16>& trackUuid,
                           const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::stopClip(const ::capnzero::SpanCL<16>& trackUuid,
                          const ::capnzero::SpanCL<16>& slot)
{
}
void SessionRpc::startClipRow(const ::capnzero::SpanCL<16>& row) {}
void SessionRpc::stopClipRow(const ::capnzero::SpanCL<16>& row) {}
