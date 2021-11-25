#ifndef BASE_SEQUENCE_H
#define BASE_SEQUENCE_H

#include <cstddef>
#include <memory_resource>
#include <list>
#include "CallbackSignal.h"


namespace base::arp
{
class BaseSequence
{
public:
   BaseSequence(std::pmr::unsynchronized_pool_resource& pool);
   void addNote(int note, float velocity);
   void clear() noexcept;
   void addPause() noexcept;
   void removeLastNote() noexcept;
   struct NoteData
   {
      static constexpr int PauseNote = -1;
      int note;
      float velocity{1.0};
      uint64_t bpmTimestamp{0};
   };

   using ContainerT = std::pmr::list<NoteData>;
   using iterator       = ContainerT::iterator;
   using const_iterator = ContainerT::const_iterator;
   using value_type     = ContainerT::value_type;
   using size_type      = ContainerT::size_type;
   inline const_iterator begin() const noexcept { return m_noteList.begin(); }
   inline const_iterator end() const noexcept { return m_noteList.end(); }
   inline size_t size() const noexcept { return m_noteList.size(); }

   CB_SIGNAL(SizeChanged, int);

private:
   ContainerT m_noteList;
   static constexpr int Threshold = 30;
};

}   // namespace base::arp

#endif