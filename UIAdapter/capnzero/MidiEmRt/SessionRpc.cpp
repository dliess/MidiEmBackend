#include "SessionRpc.h"
#include "Tracks.h"

using namespace uiadapter::capnzero;

SessionRpc::SessionRpc(base::session::Tracks& rTracks) :
    m_rTracks(rTracks)
{}

void SessionRpc::pushBackTrack(const ::capnzero::TextView& name) 
{
    m_rTracks.pushBackTrack(name);
}

void SessionRpc::addTrack(const ::capnzero::TextView& name,
                          ::capnzero::Int16 position)
{
    m_rTracks.addTrack(name, position);
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
    m_rTracks.withTrack(trackUuid, [](auto& track){track.toggleMute();});
}
void SessionRpc::createClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row)
{
    m_rTracks.withTrack(trackUuid, [row](auto& track){ track.createClip(row); });
}
void SessionRpc::copyClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                ::capnzero::Int16 srcRow,
                const ::capnzero::SpanCL<16>& destTryckUuid,
                ::capnzero::Int16 destRow)
{
    m_rTracks.withTrack(srcTrackUuid, [&, this](auto& srcTrack){
        if(srcTrack.clips[srcRow])
        {
            m_rTracks.withTrack(destTryckUuid, [&](auto& destTrack){
                destTrack.createClip(destRow, *srcTrack.clips[srcRow]);
            });
        }
    });
}
void SessionRpc::moveClip(const ::capnzero::SpanCL<16>& srcTrackUuid,
                ::capnzero::Int16 srcRow,
                const ::capnzero::SpanCL<16>& destTryckUuid,
                ::capnzero::Int16 destRow)
{
    m_rTracks.withTrack(srcTrackUuid, [&, this](auto& srcTrack){
        if(srcTrack.clips[srcRow])
        {
            m_rTracks.withTrack(destTryckUuid, [&](auto& destTrack){
                destTrack.createClip(destRow, *srcTrack.clips[srcRow]);
            });
            srcTrack.deleteClip(srcRow);
        }
    });
}
void SessionRpc::deleteClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row)
{
    m_rTracks.withTrack(trackUuid, [row](auto& track){ track.deleteClip(row); });  
}
void SessionRpc::renameClip(const ::capnzero::SpanCL<16>& trackUuid,
                            ::capnzero::Int16 row,
                            const ::capnzero::TextView& name)
{
    m_rTracks.withClip(trackUuid, row, [&name](auto& clip){
        clip.setName(name);
    });  
}
void SessionRpc::addNoteToClip(const ::capnzero::SpanCL<16>& trackUuid,
                               ::capnzero::Int16 row,
                               ::capnzero::Float32 beat,
                               const ::capnzero::SpanCL<16>& note,
                               ::capnzero::Float32 velocity)
{
    m_rTracks.withClip(trackUuid, row, [&](auto& clip){
        clip.addNote(beat, note, velocity);
    });  
}
void SessionRpc::changeNoteVelocity(const ::capnzero::SpanCL<16>& trackUuid,
                                    ::capnzero::Int16 row,
                                    ::capnzero::Float32 beat,
                                    const ::capnzero::SpanCL<16>& note,
                                    ::capnzero::Float32 velocity)
{
}
void SessionRpc::removeNoteFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                    ::capnzero::Int16 row,
                                    ::capnzero::Float32 beat,
                                    const ::capnzero::SpanCL<16>& note)
{
}
void SessionRpc::removeAllNotesFromClip(const ::capnzero::SpanCL<16>& trackUuid,
                                        ::capnzero::Int16 row)
{
}
void SessionRpc::startClip(const ::capnzero::SpanCL<16>& trackUuid,
                           ::capnzero::Int16 row)
{
}
void SessionRpc::stopClip(const ::capnzero::SpanCL<16>& trackUuid,
                          ::capnzero::Int16 row)
{
}
void SessionRpc::startClipRow(const ::capnzero::SpanCL<16>& row) {}
void SessionRpc::stopClipRow(const ::capnzero::SpanCL<16>& row) {}
