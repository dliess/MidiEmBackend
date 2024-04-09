#ifndef BASE_SESSION_TRACK_H
#define BASE_SESSION_TRACK_H

#include <cstddef>   // std::byte
#include <memory>
#include <memory_resource>
#include <optional>
#include <string_view>
#include <vector>

#include "CallbackSignal.h"
#include "Clip.h"
#include "Identifiable.h"
#include "Memory.h"
#include "NoteCollector.h"
#include "Refs/InstrumentsRef.h"

namespace base::session
{
class Track : public util::Identifiable
{
public:
   using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
   explicit Track(std::string_view name, 
                  const allocator_type& alloc = {}) noexcept;
   Track duplicate(const allocator_type& alloc) const noexcept;
   Track(Track&& rhs, const allocator_type& alloc) noexcept;
   void resetActiveClip();
   void stop();
   void update();
   inline void setName(std::string_view name);
   void toggleMute() noexcept;
   void setVolume(float volume) noexcept;
      inline void createClip(int row);
   void createClip(int row, const Clip& clip);
   void deleteClip(int row);
   void startClip(int row);
   void stopClip();
   Clip* clip(int row) noexcept;
   const Clip* clip(int row) const noexcept;
   std::optional<int> startedClipIdx() const noexcept;
   std::string_view name() const;
   void setInstrumentUUID(instruments::InstrumentsRef instrumentsRef,
                          util::Identifiable::UUIDView instrumentUUID);

   /* TODO: do we need this?
      inline void noteOn(int note, float velocity) noexcept;
      inline void noteOff(int note, float velocity) noexcept;
   */

   CB_SIGNAL_SINGLE_SUBSCRIBER(NameChanged, std::string_view);
   CB_SIGNAL_SINGLE_SUBSCRIBER(InstrumentChanged, util::Identifiable::UUIDView);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipCreated, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipDeleted, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipStartedChanged, int, bool);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipAboutToStart, int);

   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNameChanged, int, std::string_view);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteAdded, int, sequencer::NoteId,
                               sequencer::Beat, sequencer::Beat, int, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteVelocityChanged, int, sequencer::NoteId,
                               float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteLengthChanged, int, sequencer::NoteId,
                               sequencer::Beat);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteStartBeatChanged, int, sequencer::NoteId,
                               sequencer::Beat);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteRemoved, int, sequencer::NoteId);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipAllNotesRemoved, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(ClipSequenceLengthChanged, int, sequencer::Beat);

private:
   Track(const Track& rhs, const allocator_type& alloc);
   std::pmr::string m_name;
   static constexpr size_t NumClips = 64;
   std::pmr::vector<util::pmr::unique_ptr<Clip>> m_clips;
   std::optional<instruments::InstrumentRtRef> m_instrumentRef;
   static constexpr int StopperIdx = -1;
   std::optional<int> m_activeClipIdx;
   std::optional<int> m_toStartClipIdx;
   sequencer::NoteCollector m_noteCollector;
   void registerCbs(int row);

   friend void to_json(nlohmann::json& j, const Track& track);
};

}   // namespace base::session

#include "Track.inl"
#include "TrackMeta.h"

#endif
