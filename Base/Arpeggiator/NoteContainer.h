#ifndef BASE_ARP_NOTECONTAINER_H
#define BASE_ARP_NOTECONTAINER_H

#include <cstddef>
#include <list>
#include <memory_resource>

#include "CallbackSignal.h"
#include "print_alloc.h"

namespace base::arp
{
class NoteContainer
{
public:
   NoteContainer();
   void addNote(int note, float velocity) noexcept;
   void removeNote(int note) noexcept;
   void setHoldNotes(bool on) noexcept;

   [[nodiscard]] bool getHoldNotes() const noexcept { return m_holdNotes; }
   
   struct NotePress
   {
      int note;
      float velocity;
      bool released{false};
   };
   using ContainerT = std::pmr::list<NotePress>;
   using iterator       = ContainerT::iterator;
   using const_iterator = ContainerT::const_iterator;
   using value_type     = ContainerT::value_type;
   using size_type      = ContainerT::size_type;
   inline const_iterator begin() const noexcept { return m_noteList.begin(); }
   inline const_iterator end() const noexcept { return m_noteList.end(); }
   inline size_t size() const noexcept { return m_noteList.size(); }

   CB_SIGNAL(Changed, size_t, size_t);
   CB_SIGNAL(HoldNotesChanged, bool);
private:
   std::byte m_stackBuf[2048];
   util::PrintAlloc m_oom;
   std::pmr::monotonic_buffer_resource m_mbr;
   std::pmr::unsynchronized_pool_resource m_pool;
   ContainerT m_noteList;
   bool m_holdNotes{false};
   bool allReleased() const noexcept;
};

}   // namespace base::arp

#endif