#ifndef BASE_SEQUENCE_H
#define BASE_SEQUENCE_H

#include <cstddef>
#include <memory_resource>
#include <list>


namespace base::arp
{
class BaseSequence
{
public:
   BaseSequence(std::pmr::unsynchronized_pool_resource& pool);
   void push_back(int note, float velocity);
   void clear() noexcept;
   struct NoteData
   {
      int note;
      float velocity;
      int bpmTimestamp;
   };
   size_t size() const noexcept;
private:
   std::pmr::list<NoteData> m_noteList;
};

}   // namespace base::arp

#endif