#include "NoteContainer.h"
#include "loguru.hpp"

using namespace base::arp;

NoteContainer::NoteContainer() :
    m_oom("NoteContainer: Out of Memory", std::pmr::null_memory_resource()),
    m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
    m_pool(&m_mbr),
    m_noteList(&m_pool)
{
}

void NoteContainer::addNote(int note, float velocity) noexcept
{
   //LOG_F(INFO, "NoteContainer::addNote({}, {})", note, velocity);
   if(m_holdNotes && allReleased())
   {
      const size_t sizeBef = m_noteList.size();
      m_noteList.clear();
      emitChanged(sizeBef, m_noteList.size());
   }
   auto it = std::find_if(m_noteList.begin(), m_noteList.end(),
                          [note](const NotePress& notePress) -> bool {
                             return notePress.note == note;
                          });
   if (it != m_noteList.end())
   {
      it->velocity = velocity;
      emitChanged(m_noteList.size(), m_noteList.size());
   }
   else
   {
      m_noteList.emplace_back(NotePress{note, velocity});
      emitChanged(m_noteList.size() - 1, m_noteList.size());
   }
}

void NoteContainer::removeNote(int note) noexcept
{
   //LOG_F(INFO, "NoteContainer::removeNote({})", note);
   auto it = std::find_if(m_noteList.begin(), m_noteList.end(),
                          [note](const NotePress& notePress) -> bool {
                             return notePress.note == note;
                          });
    if(it != m_noteList.end())
    {
      if(m_holdNotes)
      {
         it->released = true;
      }
      else
      {
        m_noteList.erase(it);
        emitChanged(m_noteList.size() + 1, m_noteList.size());
      }
    }
}

void NoteContainer::setHoldNotes(bool on) noexcept
{
   if(m_holdNotes != on)
   {
      m_holdNotes = on;
      if(false == m_holdNotes)
      {
         const size_t sizeBef = m_noteList.size();
         for(auto it = m_noteList.begin(); it != m_noteList.end(); ++it)
         {
            if(it->released)
            {
               it = m_noteList.erase(it);
            }
         }
         if(sizeBef != m_noteList.size())
         {
            emitChanged(sizeBef, m_noteList.size());
         }
      }
      emitHoldNotesChanged(on);
   }
}

bool NoteContainer::allReleased() const noexcept
{
   bool allReleased = true;
   for(const auto& e : m_noteList)
   {
      if(!e.released) allReleased = false;
   }
   return allReleased;
}
