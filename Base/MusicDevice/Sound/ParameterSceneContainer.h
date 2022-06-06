#ifndef BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H
#define BASE_MUSIC_DEVICE_SOUND_PARAMETER_SCENE_CONTAINER_H

#include "ParameterScene.h"

#include <list>
#include <memory_resource>
#include "stack_mempool.h"

namespace base::musicDevice::sound
{

class ParameterSceneContainer
{
public:
   ParameterSceneContainer() :
       m_memoryPool("ParameterSceneContainer"),
       m_data(&m_memoryPool.pool())
   {
   }
   using ContainerT = std::pmr::list<ParameterScene>;
   const ContainerT& data() const noexcept { return m_data; }
   ContainerT& data() noexcept { return m_data; }
private:
    util::StackMempool<32768> m_memoryPool;
    ContainerT m_data;
};

}

#endif