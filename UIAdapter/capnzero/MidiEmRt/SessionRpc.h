#ifndef SESSION_RPC_H
#define SESSION_RPC_H

#include "MidiEmRtSessionRpcIf.h"

namespace base::session
{
struct Tracks;
}

namespace uiadapter::capnzero
{
class SessionRpc : public ::capnzero::MidiEmRt::SessionRpcIf
{
public:
   SessionRpc(base::session::Tracks& rTracks);
   void pushBackTrack(const ::capnzero::TextView& name) override;
   void addTrack(const ::capnzero::TextView& name,
                 ::capnzero::Int16 position) override;
   void duplicateTrack(const ::capnzero::SpanCL<16>& trackUuid) override;
   void removeTrack(const ::capnzero::SpanCL<16>& trackUuid) override;
   void renameTrack(const ::capnzero::SpanCL<16>& trackUuid,
                    const ::capnzero::TextView& name) override;
   void moveTrack(const ::capnzero::SpanCL<16>& trackUuid,
                  ::capnzero::Int16 afterPosition) override;
   void toggleTrackMute(const ::capnzero::SpanCL<16>& trackUuid) override;
   void createClip(const ::capnzero::SpanCL<16>& trackUuid,
                   ::capnzero::Int16 row) override;
   void copyClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                 ::capnzero::Int16 srcRow,
                 const ::capnzero::SpanCL<16>& destTryckUuid,
                 ::capnzero::Int16 destRow) override;
   void moveClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                 ::capnzero::Int16 srcRow,
                 const ::capnzero::SpanCL<16>& destTryckUuid,
                 ::capnzero::Int16 destRow) override;
   void deleteClip(const ::capnzero::SpanCL<16>& trackUuid,
                   ::capnzero::Int16 row) override;
   void renameClip(const ::capnzero::SpanCL<16>& trackUuid,
                   ::capnzero::Int16 row,
                   const ::capnzero::TextView& name) override;
   void addNoteToClip(const ::capnzero::SpanCL<16>& trackUuid,
                      ::capnzero::Int16 row, ::capnzero::Float32 beat,
                      const ::capnzero::SpanCL<16>& note,
                      ::capnzero::Float32 velocity) override;
   void changeNoteVelocity(const ::capnzero::SpanCL<16>& trackUuid,
                           ::capnzero::Int16 row, ::capnzero::Float32 beat,
                           const ::capnzero::SpanCL<16>& note,
                           ::capnzero::Float32 velocity) override;
   void removeNoteFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                           ::capnzero::Int16 row, ::capnzero::Float32 beat,
                           const ::capnzero::SpanCL<16>& note) override;
   void removeAllNotesFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                               ::capnzero::Int16 row) override;
   void startClip(const ::capnzero::SpanCL<16>& trackUuid,
                  ::capnzero::Int16 row) override;
   void stopClip(const ::capnzero::SpanCL<16>& trackUuid,
                 ::capnzero::Int16 row) override;
   void startClipRow(const ::capnzero::SpanCL<16>& row) override;
   void stopClipRow(const ::capnzero::SpanCL<16>& row) override;

private:
   base::session::Tracks& m_rTracks;
};

}   // namespace uiadapter::capnzero

#endif