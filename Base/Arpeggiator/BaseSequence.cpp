#include "BaseSequence.h"

#include "BeatTick.h"
#include "loguru.hpp"

using namespace base::arp;

BaseSequence::BaseSequence(std::pmr::unsynchronized_pool_resource& pool) :
    m_noteList(&pool)
{
}

void BaseSequence::addNote(int note, float velocity)
{
   uint64_t ts_now = base::tempo::BeatTick::instance().getBeatJiffies();
   if (m_noteList.size() && m_noteList.back().note == NoteData::PauseNote &&
       ((ts_now - m_noteList.back().bpmTimestamp) < Threshold))
   {
      m_noteList.back() = NoteData{note, velocity, ts_now};
   }
   else
   {
      m_noteList.emplace_back(NoteData{note, velocity, ts_now});
      emitSizeChanged(size());
   }
}

void BaseSequence::clear() noexcept
{
   m_noteList.clear();
   emitSizeChanged(size());
}

void BaseSequence::addPause() noexcept
{
   uint64_t ts_now = base::tempo::BeatTick::instance().getBeatJiffies();
   if (m_noteList.size() && m_noteList.back().note != NoteData::PauseNote &&
       ((ts_now - m_noteList.back().bpmTimestamp) < Threshold))
   {
      return;
   }
   m_noteList.emplace_back(NoteData{NoteData::PauseNote, 0.0, ts_now});
   emitSizeChanged(size());
}

void BaseSequence::removeLastNote() noexcept
{
   m_noteList.pop_back();
   emitSizeChanged(size());
}