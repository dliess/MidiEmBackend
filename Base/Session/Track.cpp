#include "Track.h"

#include "BeatTick.h"

using namespace base;

session::Track::Track(std::string_view name,
                      const allocator_type& alloc) noexcept :
    m_name(name, alloc), m_clips(NumClips, alloc)
{
}

session::Track::Track(const Track& rhs, const allocator_type& alloc) :
    m_name(rhs.m_name, alloc)
{
   m_clips.resize(rhs.m_clips.size());
   for (int i = 0; i < rhs.m_clips.size(); ++i)
   {
      if (rhs.m_clips[i])
      {
         Clip clip  = *rhs.m_clips[i];
         m_clips[i] = util::pmr::make_unique(clip, alloc);
      }
   }
}

session::Track::Track(Track&& rhs, const allocator_type& alloc) noexcept :
    m_name(std::move(rhs.m_name), alloc), m_clips(std::move(rhs.m_clips), alloc)
{
}

session::Track session::Track::duplicate(
    const allocator_type& alloc) const noexcept
{
   return Track(*this, alloc);
}

void session::Track::resetActiveClip()
{
   if (m_activeClipIdx)
   {
      m_clips[m_activeClipIdx.value()]->reset();
   }
}

void session::Track::stop()
{
   if (m_activeClipIdx)
   {
      m_clips[m_activeClipIdx.value()]->stop(m_instrument);
   }
}

void session::Track::update()
{
   if (m_toStartClipIdx)
   {
      static constexpr double Threshold = 0.1;
      const double inBeatPos = tempo::BeatTick::instance().getLocalInBeatPos();
      if (inBeatPos < Threshold)
      {
         if (m_activeClipIdx)
         {
            m_clips[m_activeClipIdx.value()]->stop(m_instrument);
         }
         if (StopperIdx != m_toStartClipIdx.value())
         {
            m_clips[m_toStartClipIdx.value()]->reset();
            m_activeClipIdx = m_toStartClipIdx;
         }
         else
         {
            m_activeClipIdx.reset();
         }
         m_toStartClipIdx.reset();
      }
   }
   if (m_activeClipIdx)
   {
      m_clips[m_activeClipIdx.value()]->update(m_instrument);
   }
}

void session::Track::toggleMute() noexcept
{
   // TODO
}

void session::Track::registerCbs(int row)
{
   m_clips[row]->onNameChanged(
       [this, row](std::string_view name) { emitClipNameChanged(row, name); });
   m_clips[row]->onNoteAdded(
       [this, row](sequencer::NoteId noteId, sequencer::Beat start,
                   sequencer::Beat length, int note, float velocity) {
          emitClipNoteAdded(row, noteId, start, length, note, velocity);
       });
   m_clips[row]->onNoteVelocityChanged(
       [this, row](sequencer::NoteId noteId, float velocity) {
          emitClipNoteVelocityChanged(row, noteId, velocity);
       });
   m_clips[row]->onNoteLengthChanged(
       [this, row](sequencer::NoteId noteId, sequencer::Beat length) {
          emitClipNoteLengthChanged(row, noteId, length);
       });
   m_clips[row]->onNoteStartBeatChanged(
       [this, row](sequencer::NoteId noteId, sequencer::Beat startBeat) {
          emitClipNoteStartBeatChanged(row, noteId, startBeat);
       });
   m_clips[row]->onNoteRemoved([this, row](sequencer::NoteId noteId) {
      emitClipNoteRemoved(row, noteId);
   });
   m_clips[row]->onAllNotesRemoved(
       [this, row]() { emitClipAllNotesRemoved(row); });
}