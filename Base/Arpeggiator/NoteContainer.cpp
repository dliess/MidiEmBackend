#include "NoteContainer.h"

using namespace base::arp;

NoteContainer::NoteContainer() :
    m_oom("Out of Memory", std::pmr::null_memory_resource()),
    m_mbr(m_stackBuf, sizeof(m_stackBuf), &m_oom),
    m_pool(&m_mbr),
    m_noteList(&m_pool)
{
}

void NoteContainer::addNote(int note, float velocity) noexcept
{
   auto it = std::find_if(m_noteList.begin(), m_noteList.end(),
                          [note](const NotePress& notePress) -> bool {
                             return notePress.note == note;
                          });
   if (it == m_noteList.end())
   {
      it->velocity = velocity;
   }
   else
   {
      m_noteList.emplace_back(NotePress{note, velocity});
      if(size() == 1) emitGotFirstNote();
   }
   emitChanged();
}

void NoteContainer::removeNote(int note) noexcept
{
   auto it = std::find_if(m_noteList.begin(), m_noteList.end(),
                          [note](const NotePress& notePress) -> bool {
                             return notePress.note == note;
                          });
    if(it != m_noteList.end())
    {
        m_noteList.erase(it);
        if(size() == 0) emitGotEmpty();
    }
    emitChanged();
}

void NoteContainer::reEmitSignals() noexcept
{
   if(size())
   {

   }
   else
   {
      
   }
}

