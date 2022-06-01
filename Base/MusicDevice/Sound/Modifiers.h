#ifndef BASE_MUSIC_DEVICE_SOUND_MODIFIERS_H
#define BASE_MUSIC_DEVICE_SOUND_MODIFIERS_H

#include <list>
#include <memory_resource>
#include "stack_mempool.h"
#include "Identifiable.h"
#include "ParameterPart.h"

namespace base::musicDevice::sound
{
class Modifiers
{
public:
   Modifiers(std::pmr::unsynchronized_pool_resource& pool) :
       m_modifierList(&m_memoryPool.pool())
   {
   }
   struct Modifier
   {
      float goalValue{0};
      float intensity{0};
      struct Destination
      {
         util::Identifiable::UUID uuid;
         int voiceIdx{0};
         int parameterIdx{0};
         ParameterPart parameterPart{ParameterPart::Commanded};
      };
      parameter
   };
   using ContainerT     = std::pmr::list<Modifier>;
   using iterator       = ContainerT::iterator;
   using const_iterator = ContainerT::const_iterator;
   using value_type     = ContainerT::value_type;
   using size_type      = ContainerT::size_type;
   inline const_iterator begin() const noexcept
   {
      return m_modifierList.begin();
   }
   inline const_iterator end() const noexcept { return m_modifierList.end(); }
   inline size_t size() const noexcept { return m_modifierList.size(); }

private:
   util::StackMempool<32768> m_memoryPool;
   ContainerT m_modifierList;
};

}   // namespace base::musicDevice::sound

#endif