#include "MelodicInstrument.h"

#include "MusicDeviceHolder.h"
#include "SoundSection.h"
#include "UtilVectorIndexInRange.h"

using namespace base::instruments::loader;

MelodicInstrument::MelodicInstrument(std::string name) noexcept :
    m_name(std::move(name))
{
}

void MelodicInstrument::setParameterValue(
    int componentIdx, int parameterId,
    musicDevice::sound::ParameterAttr parameterAttr, float value) const
{
   for (auto& voice : m_voices)
   {
      if (mddescrutil::vector_index_in_range(componentIdx, voice.components))
      {
         auto& component = voice.components[componentIdx];
         if (component)
         {
            component->setParameterValue(parameterId, parameterAttr, value);
         }
      }
   }
}


const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int componentIdx,
                                        int parameterIdx) const
{
   auto component = getFirstComponent(componentIdx);
   if (component)
   {
      return component->parameterDescription(parameterIdx);
   }
   return nullptr;   // TODO: exception?
}

std::string MelodicInstrument::name() const noexcept { return m_name; }

void MelodicInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}
