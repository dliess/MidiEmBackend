#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_DETAIL_INL
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_DETAIL_INL

#include "ControllerEventRouterRt.h"

#include "ControllerHandler.h"
#include "KitInstrument.h"
#include "Rt/MelodicInstrument.h"
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
void setParameter4PressRelease(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::PressReleaseType& value,
                  MDCoords... mdCoords)
{
   if (!isPress(value)) return;
   if (isList(parameter))
   {
      const float incr =
            parameter.descriptionCache.upwards ? 1 : -1;
      dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr, 
                                  musicDevice::sound::IncrementMode::RoundRobin);
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
         parameter.valueCache->valueAtPress = actualVal.value();
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

inline float calcIncrements(const EventDestination::Parameter& parameter, 
                          const controller::IncrementType& increment)
{
   if (isList(parameter))
   {
      const int accIncr = increment.value + parameter.valueCache->storedIncrements;
      const int incrForOneStep        = increment.resolution / 12;
      parameter.valueCache->storedIncrements = accIncr % incrForOneStep;
      return float(accIncr / incrForOneStep);
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
   dev.fromNormalizedValue(mdCoords..., parameter.id, parameter.parameterAttr, value.value).map([&](float val) {
      if(parameter.descriptionCache.eventBound)
      {
         dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, val);
      }
      else
      {
         const auto actualVal = dev.getParameterValue(mdCoords..., parameter.id, parameter.parameterAttr);
         if(actualVal && isNearEnough(actualVal.value(), val))
         {
            dev.setParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, val);
         }
      }
   });
}
void setParameterMPEForContinousValue(instruments::MelodicInstrumentRtRef dev, const EventDestination::Parameter& parameter,
                     const controller::ContinousValueType& value, int note, int componentIdx)
{
   dev.fromNormalizedValue(note, componentIdx, parameter.id, parameter.parameterAttr, value.value).map([&dev, note, componentIdx, parameter](float val) {
      dev.setParameterValueMPE(note, componentIdx, parameter.id, parameter.parameterAttr, val);
   });
}

template <typename Dev, typename... MDCoords>
void setParameterForIncrement(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::IncrementType& increment,
                  MDCoords... mdCoords)
{
   const float incr = calcIncrements(parameter, increment);
   if(parameter.descriptionCache.eventBound)
   {
      dev.incrementParameterValueEventBound(mdCoords..., parameter.id, parameter.parameterAttr, incr, 
                                             musicDevice::sound::IncrementMode::Limit);
   }
   else
   {
      dev.incrementParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, incr,
                                 musicDevice::sound::IncrementMode::Limit);
   }
}


template <typename Dev, typename... MDCoords>
void setParameterForRelativeValue(Dev& dev, const EventDestination::Parameter& parameter,
                  const controller::RelativeValueType& value,
                  MDCoords... mdCoords)
{
   dev.setRelativeParameterValue(mdCoords..., parameter.id, parameter.parameterAttr, value.value);
}

void setParameterMPERelativeValue(instruments::MelodicInstrumentRtRef dev, const EventDestination::Parameter& parameter,
                     const controller::RelativeValueType& value, int note,
                     int componentIdx)
{
   const auto actValue = dev.getParameterValue(note, componentIdx, parameter.id, parameter.parameterAttr);
   if(actValue)
   {
      dev.setParameterValueMPE(note, componentIdx, parameter.id, parameter.parameterAttr, actValue.value() + value.value);
   }
}

}   // namespace base::eventRouter::detail

#endif
