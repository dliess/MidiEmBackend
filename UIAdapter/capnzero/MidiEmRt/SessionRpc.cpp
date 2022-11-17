#include "SessionRpc.h"

#include "Tracks.h"

using namespace uiadapter::capnzero;

SessionRpc::SessionRpc(base::session::Tracks& rTracks) : m_rTracks(rTracks) {}

void SessionRpc::pushBackTrack(const ::capnzero::TextView& name)
{
   m_rTracks.pushBackTrack(name);
}

void SessionRpc::pushBackTrackWithInstrument(
    const ::capnzero::TextView& name,
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   m_rTracks.pushBackTrack(name, instrumentUuid);
}

void SessionRpc::addTrack(const ::capnzero::TextView& name,
                          ::capnzero::Int16 position)
{
   m_rTracks.addTrack(name, position);
}

void SessionRpc::addTrackWithInstrument(
    const ::capnzero::TextView& name, ::capnzero::Int16 position,
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   m_rTracks.addTrack(name, position, instrumentUuid);
}

void SessionRpc::changeTrackInstrument(
    const ::capnzero::SpanCL<16>& trackUuid,
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   m_rTracks.setTrackInstrument(trackUuid, instrumentUuid);
}

void SessionRpc::duplicateTrack(const ::capnzero::SpanCL<16>& trackUuid)
{
   m_rTracks.duplicateTrack(trackUuid);
}

void SessionRpc::removeTrack(const ::capnzero::SpanCL<16>& trackUuid)
{
   m_rTracks.removeTrack(trackUuid);
}
void SessionRpc::renameTrack(const ::capnzero::SpanCL<16>& trackUuid,
                             const ::capnzero::TextView& name)
{
   m_rTracks.renameTrack(trackUuid, name);
}
void SessionRpc::moveTrack(const ::capnzero::SpanCL<16>& trackUuid,
                           ::capnzero::Int16 afterPosition)
{
   m_rTracks.moveTrack(trackUuid, afterPosition);
}

void SessionRpc::toggleTrackMute(const ::capnzero::SpanCL<16>& trackUuid)
{
   m_rTracks.withTrack(trackUuid, [](auto& track) { track.toggleMute(); });
}
void SessionRpc::createClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row)
{
   m_rTracks.withTrack(trackUuid,
                       [row](auto& track) { track.createClip(row); });
}
void SessionRpc::copyClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                          ::capnzero::Int16 srcRow,
                          const ::capnzero::SpanCL<16>& destTryckUuid,
                          ::capnzero::Int16 destRow)
{
   m_rTracks.withTrack(srcTrackUuid, [&, this](auto& srcTrack) {
      if (srcTrack.clip(srcRow))
      {
         m_rTracks.withTrack(destTryckUuid, [&](auto& destTrack) {
            destTrack.createClip(destRow, *srcTrack.clip(srcRow));
         });
      }
   });
}
void SessionRpc::moveClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                          ::capnzero::Int16 srcRow,
                          const ::capnzero::SpanCL<16>& destTryckUuid,
                          ::capnzero::Int16 destRow)
{
   m_rTracks.withTrack(srcTrackUuid, [&, this](auto& srcTrack) {
      if (srcTrack.clip(srcRow))
      {
         m_rTracks.withTrack(destTryckUuid, [&](auto& destTrack) {
            destTrack.createClip(destRow, *srcTrack.clip(srcRow));
         });
         srcTrack.deleteClip(srcRow);
      }
   });
}
void SessionRpc::deleteClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row)
{
   m_rTracks.withTrack(trackUuid,
                       [row](auto& track) { track.deleteClip(row); });
}
void SessionRpc::renameClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row,
                            const ::capnzero::TextView& name)
{
   m_rTracks.withClip(trackUuid, row,
                      [&name](auto& clip) { clip.setName(name); });
}
void SessionRpc::addNoteToClip(const ::capnzero::SpanCL<16>& trackUuid,
                               ::capnzero::Int16 row, ::capnzero::Float32 beat,
                               ::capnzero::Float32 length,
                               ::capnzero::Int16 note,
                               ::capnzero::Float32 velocity)
{
   m_rTracks.withClip(trackUuid, row, [&](auto& clip) {
      clip.addNote(beat, length, note, velocity);
   });
}
void SessionRpc::changeNoteVelocity(const ::capnzero::SpanCL<16>& trackUuid,
                                    ::capnzero::Int16 row,
                                    ::capnzero::UInt32 noteId,
                                    ::capnzero::Float32 velocity)
{
   m_rTracks.withClip(trackUuid, row, [&](auto& clip) {
      clip.setNoteVelocity(noteId, velocity);
   });
}
void SessionRpc::changeNoteLength(const ::capnzero::SpanCL<16>& trackUuid,
                        ::capnzero::Int16 row, ::capnzero::UInt32 noteId,
                        ::capnzero::Float32 length)
{
   m_rTracks.withClip(trackUuid, row, [&](auto& clip) {
      clip.setNoteLength(noteId, length);
   });
}
void SessionRpc::changeNoteStartBeat(const ::capnzero::SpanCL<16>& trackUuid,
                        ::capnzero::Int16 row, ::capnzero::UInt32 noteId,
                        ::capnzero::Float32 startBeat)
{
   m_rTracks.withClip(trackUuid, row, [&](auto& clip) {
      clip.setNoteStartBeat(noteId, startBeat);
   });
}

void SessionRpc::removeNoteFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                    ::capnzero::Int16 row,
                                    ::capnzero::UInt32 noteId)
{
   m_rTracks.withClip(trackUuid, row,
                      [&](auto& clip) { clip.removeNote(noteId); });
}
void SessionRpc::removeAllNotesFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                        ::capnzero::Int16 row)
{
   m_rTracks.withClip(trackUuid, row,
                      [&](auto& clip) { clip.removeAllNotes(); });
}

void SessionRpc::changeSequenceLength(const ::capnzero::SpanCL<16>& trackUuid,
                              ::capnzero::Int16 row, ::capnzero::Float32 sequenceLength)
{
   m_rTracks.withClip(trackUuid, row,
                      [&](auto& clip) { clip.setSequenceLength(sequenceLength); });
}

void SessionRpc::startClip(const ::capnzero::SpanCL<16>& trackUuid,
                           ::capnzero::Int16 row)
{
   m_rTracks.withTrack(trackUuid, [&](auto& track) { track.startClip(row); });
}
void SessionRpc::stopTrack(const ::capnzero::SpanCL<16>& trackUuid)
{
   m_rTracks.withTrack(trackUuid, [&](auto& track) { track.stopClip(); });
}
void SessionRpc::startClipRow(::capnzero::Int16 row)
{
   m_rTracks.startClipRow(row);
}