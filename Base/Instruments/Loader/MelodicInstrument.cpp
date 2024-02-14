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
    musicDevice::sound::ParameterAttr parameterAttr, float value)
{
   if(m_parameters.at(componentIdx))
   {
      musicDevice::sound::setParameterData(m_parameters.at(componentIdx)->deviceParameters
                                                       .at(parameterId), 
                                           parameterAttr, value);
   }
}

const base::musicDevice::description::sound::Parameter*
MelodicInstrument::parameterDescription(int componentIdx,
                                        int parameterIdx) const
{
   if(m_parameters.at(componentIdx))
   {
      return &m_parameters.at(componentIdx)->pEngineDescr->parameters.at(parameterIdx);
   }
   return nullptr;
}

std::string MelodicInstrument::name() const noexcept { return m_name; }

void MelodicInstrument::setName(const std::string& name) noexcept
{
   m_name = name;
}

