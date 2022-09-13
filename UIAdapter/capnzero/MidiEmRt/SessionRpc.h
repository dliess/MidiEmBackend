#ifndef SESSION_RPC_H
#define SESSION_RPC_H

#include "MidiEmRtSessionRpcIf.h"

namespace uiadapter::capnzero
{
class SessionRpc : public ::capnzero::MidiEmRt::SessionRpcIf
{
public:
   void pushBackTrack(const ::capnzero::TextView& name) override;
   void addTrack(const ::capnzero::TextView& name,
                 ::capnzero::Int16 position) override;
   void duplicateTrack(const ::capnzero::SpanCL<16>& trackUuid) override;
   void removeTrack(const ::capnzero::SpanCL<16>& trackUuid) override;
   void renameTrack(const ::capnzero::SpanCL<16>& trackUuid,
                    const ::capnzero::TextView& name) override;
   void moveTrack(const ::capnzero::SpanCL<16>& track1Uuid,
                  ::capnzero::Int16 afterPosition) override;
   void toggleTrackMute(const ::capnzero::SpanCL<16>& track1Uuid) override;
   void createClip(const ::capnzero::SpanCL<16>& trackUuid,
                   const ::capnzero::SpanCL<16>& slot) override;
   void duplicateClip(const ::capnzero::SpanCL<16>& trackUuid,
                      const ::capnzero::SpanCL<16>& slot) override;
   void deleteClip(const ::capnzero::SpanCL<16>& trackUuid,
                   const ::capnzero::SpanCL<16>& slot) override;
   void renameClip(const ::capnzero::SpanCL<16>& trackUuid,
                   const ::capnzero::SpanCL<16>& slot,
                   const ::capnzero::TextView& name) override;
   void addNoteToClip(const ::capnzero::SpanCL<16>& trackUuid,
                      const ::capnzero::SpanCL<16>& slot,
                      ::capnzero::Float32 beat,
                      const ::capnzero::SpanCL<16>& note,
                      ::capnzero::Float32 velocity) override;
   void changeNoteVelocity(const ::capnzero::SpanCL<16>& trackUuid,
                           const ::capnzero::SpanCL<16>& slot,
                           ::capnzero::Float32 beat,
                           const ::capnzero::SpanCL<16>& note,
                           ::capnzero::Float32 velocity) override;
   void removeNoteFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                           const ::capnzero::SpanCL<16>& slot,
                           ::capnzero::Float32 beat,
                           const ::capnzero::SpanCL<16>& note) override;
   void removeAllNotesFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                               const ::capnzero::SpanCL<16>& slot) override;
   void moveClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                 const ::capnzero::SpanCL<16>& srcSlot,
                 const ::capnzero::SpanCL<16>& destTrackUuid,
                 const ::capnzero::SpanCL<16>& destSlot) override;
   void startClip(const ::capnzero::SpanCL<16>& trackUuid,
                  const ::capnzero::SpanCL<16>& slot) override;
   void stopClip(const ::capnzero::SpanCL<16>& trackUuid,
                 const ::capnzero::SpanCL<16>& slot) override;
   void startClipRow(const ::capnzero::SpanCL<16>& row) override;
   void stopClipRow(const ::capnzero::SpanCL<16>& row) override;
};

}   // namespace uiadapter::capnzero

#endif