#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_DETAIL_INL
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_DETAIL_INL

#include "ControllerEventRouterRt.h"

#include "ControllerHandler.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"
#include "SoundHandler.h"

using namespace base;
using namespace base::eventRouter;
using namespace base::musicDevice;


namespace base::eventRouter::detail
{
template <typename Dev, typename... DevCoord>
void playNoteOnOff(Dev& dev, int note, float velocity,
                   const DevCoord&... devCoord)
{
   if (velocity > 0)
   {
      dev.noteOn(devCoord..., note, velocity);
   }
   else
   {
      dev.noteOff(devCoord..., note, -velocity);
   }
}

bool isList(const EventDestination::Parameter& parameter) {
   return (parameter.parameterAttr == ParameterAttr::Commanded && parameter.descriptionCache.isList) || 
          parameter.parameterAttr == ParameterAttr::LfoMultiplierExp ||
          parameter.parameterAttr == ParameterAttr::LfoWaveform;
}

bool isPress(const controller::PressReleaseType& value) {
   return value.value > 0;
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::PressReleaseType& value,
                  MDCoords... mdCoords)
{
   if (!isPress(value)) return;
   if (isList(parameter))
   {
      const float incr =
            parameter.descriptionCache.upwards ? value.value : -value.value;
      dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr, true);
   }
   else
   {
      const auto actualVal =
            dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      if(!actualVal) {
         return;
      }
      if (std::fabs(actualVal.value() - parameter.descriptionCache.zeroVal) <
            std::numeric_limits<float>::epsilon())
      {
         if (parameter.valueCache->valueAtPress)
         {
            dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr,
                                    *parameter.valueCache->valueAtPress);
         }
      }
      else
      {
         parameter.valueCache->valueAtPress = actualVal;
         dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr,
                                 parameter.descriptionCache.zeroVal);
      }
   }
}

inline
bool isNearEnough(float newVal, float actualVal)
{
   const float diff = std::fabs(newVal - actualVal);
   return ((diff != 0) && (diff < 0.12 || diff >= 1.0));
}

inline int calcIncrements(const EventDestination::Parameter& parameter, 
                          const controller::IncrementType& increment)
{
   if (isList(parameter))
   {
      const int accIncr = increment.value + parameter.valueCache->storedIncrements;
      const int incrForOneStep        = increment.resolution / 12;
      parameter.valueCache->storedIncrements = accIncr % incrForOneStep;
      return accIncr / incrForOneStep;
   }
   else
   {   // TODO: highres mode
      return float(increment.value) / float(increment.resolution);
   }
}


template <typename Dev, typename... MDCoords>
void setParameterForContinousValue(Dev& dev, const EventDestination::Parameter& parameter,
                                   const controller::ContinousValueType& value,
                                   MDCoords... mdCoords)
{
   const float val = dev.fromNormalizedValue(mdCoords..., parameter.id, parameter.parameterAttr, value.value);
   const auto actualVal = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
   if(actualVal && isNearEnough(actualVal.value(), val))
   {
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, val);
   }
}
void setParameterMPEForContinousValue(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                     const controller::ContinousValueType& value, int note, int componentIdx)
{
   const float val = dev.fromNormalizedValue(note, componentIdx, parameter.id, parameter.parameterAttr, value.value);
   dev.setParameterValueMPE(note, componentIdx, parameter.id, parameter.parameterAttr, val);
}

template <typename Dev, typename... MDCoords>
void setParameterForIncrement(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::IncrementType& increment,
                  MDCoords... mdCoords)
{
   const int incr = calcIncrements(parameter, increment);
   dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr, false);
}


template <typename Dev, typename... MDCoords>
void setParameterForRelativeValue(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeValueType& value,
                  MDCoords... mdCoords)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, valueToSet);
   }
}

void setParameterMPE(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                     const controller::RelativeValueType& value, int note,
                     int componentIdx)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValueMPE(note, componentIdx, parameter.id, parameter.parameterAttr, valueToSet);
   }
}

template <typename Dev, typename... MDCoords>
void setParameter(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeUnlimitedValueType& value,
                  MDCoords... mdCoords)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, valueToSet);
   }
}
void setParameterMPE(const instruments::MelodicInstrument& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeUnlimitedValueType& value, int note,
                  int componentIdx)
{
   if (0 == value.value || !parameter.valueCache->valueAtPress)
   {
      const auto actValue = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
      parameter.valueCache->valueAtPress = actValue;
   }
   if(parameter.valueCache->valueAtPress)
   {
      const float valueToSet = parameter.valueCache->valueAtPress.value() + value.value;
      dev.setParameterValueMPE(note, componentIdx, parameter.id, parameter.parameterAttr, valueToSet);
   }
}


}   // namespace base::eventRouter::detail

#endif