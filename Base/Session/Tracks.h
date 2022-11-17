#ifndef BASE_SESSION_TRACKS_H
#define BASE_SESSION_TRACKS_H

#include <string_view>
#include <vector>

#include "CallbackSignal.h"
#include "Identifiable.h"
#include "Track.h"
#include "stack_mempool.h"

namespace base::instruments
{
class Instruments;
}

namespace base::session
{
class Tracks
{
public:
   Tracks(instruments::Instruments& rInstruments);
   void start();
   void stop();
   void update();
   Track& pushBackTrack(std::string_view name);
   void pushBackTrack(std::string_view name,
                      util::Identifiable::UUIDView instrumentUuid);
   Track& addTrack(std::string_view name, int position);
   void addTrack(std::string_view name, int position,
                 util::Identifiable::UUIDView instrumentUuid);
   void duplicateTrack(util::Identifiable::UUIDView uuid);
   void removeTrack(util::Identifiable::UUIDView uuid);
   void renameTrack(util::Identifiable::UUIDView uuid, std::string_view name);
   void setTrackInstrument(util::Identifiable::UUIDView trackUuid,
                           util::Identifiable::UUIDView instrumentUuid);
   void moveTrack(util::Identifiable::UUIDView uuid, int afterPosition);
   template <typename CB>
   void withTrack(util::Identifiable::UUIDView uuid, CB&& cb);
   template <typename CB>
   void withClip(util::Identifiable::UUIDView uuid, int row, CB&& cb);
   void startClipRow(int row);

   CB_SIGNAL(TrackAdded, util::Identifiable::UUIDView, std::string_view, int);
   CB_SIGNAL(TrackRemoved, util::Identifiable::UUIDView);
   CB_SIGNAL(TrackDuplicated, util::Identifiable::UUIDView,
             util::Identifiable::UUIDView);
   CB_SIGNAL(TrackMoved, util::Identifiable::UUIDView, int);
   CB_SIGNAL(TrackMuted, util::Identifiable::UUIDView, bool);

   CB_SIGNAL(TrackNameChanged, util::Identifiable::UUIDView, std::string_view);
   CB_SIGNAL(TrackInstrumentChanged, util::Identifiable::UUIDView,
             util::Identifiable::UUIDView);
   CB_SIGNAL(TrackClipCreated, util::Identifiable::UUIDView, int);
   CB_SIGNAL(TrackClipDeleted, util::Identifiable::UUIDView, int);
   CB_SIGNAL(TrackClipStartedChanged, util::Identifiable::UUIDView, int, bool);

   CB_SIGNAL(TrackClipNameChanged, util::Identifiable::UUIDView, int,
             std::string_view);
   CB_SIGNAL(TrackClipNoteAdded, util::Identifiable::UUIDView, int,
             sequencer::NoteId, sequencer::Beat, sequencer::Beat, int, float);
   CB_SIGNAL(TrackClipNoteVelocityChanged, util::Identifiable::UUIDView, int,
             sequencer::NoteId, float);
   CB_SIGNAL(TrackClipNoteLengthChanged, util::Identifiable::UUIDView, int,
             sequencer::NoteId, sequencer::Beat);
   CB_SIGNAL(TrackClipNoteStartBeatChanged, util::Identifiable::UUIDView, int,
             sequencer::NoteId, sequencer::Beat);
   CB_SIGNAL(TrackClipNoteRemoved, util::Identifiable::UUIDView, int,
             sequencer::NoteId);
   CB_SIGNAL(TrackClipAllNotesRemoved, util::Identifiable::UUIDView, int);
   CB_SIGNAL(TrackClipSequenceLengthChanged, util::Identifiable::UUIDView, int, sequencer::Beat);

private:
   instruments::Instruments& m_rInstruments;
   util::StackMempool<320768> m_memoryPool;
   template <typename CB>
   void withTrackIter(util::Identifiable::UUIDView uuid, CB&& cb);
   std::pmr::vector<Track> m_tracks;
   bool m_started{false};
   void registerCbs(Track& track);
};

}   // namespace base::session

#include "Tracks.inl"

#endif