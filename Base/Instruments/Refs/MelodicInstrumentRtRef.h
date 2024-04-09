#ifndef MELODIC_INSTRUMENT_RT_REF_H
#define MELODIC_INSTRUMENT_RT_REF_H

#include <array>
#include <cstddef>
#include "ErrorHandling.h"
#include "ParameterData.h"

namespace base::instruments
{

namespace detail
{
class MelodicInstrumentRtRefConcept
{
public:
   virtual ~MelodicInstrumentRtRefConcept() = default;
   virtual Void noteOn(int note, float velocity, void* token) = 0;
   virtual Void noteOff(int note, float velocity, void* token) = 0;
   virtual void pitchBend(float value) = 0;
   virtual Void pitchBendMPE(int note, float value) = 0;
   virtual Void incrementParameterValue(int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) = 0;
   virtual Void incrementParameterValueEventBound(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float increment,
       musicDevice::sound::IncrementMode incrementMode) = 0;
   virtual Ret<float> getParameterValue(
       int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const = 0;
   virtual Ret<float> getParameterValue(
       int note, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const = 0;
   virtual Void setParameterValue(int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) = 0;
   virtual Ret<float> fromNormalizedValue(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const = 0;
   virtual Ret<float> fromNormalizedValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const = 0;
   virtual Void setParameterValueMPE(int note, int componentIdx, int parameterId,
                             musicDevice::sound::ParameterAttr parameterAttr,
                             float value) = 0;
   virtual Void setRelativeParameterValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue) = 0;
};

template <class MelodicInstrumentRtRefImpl>
class MelodicInstrumentRtRefAdaptor : public MelodicInstrumentRtRefConcept
{
public:
   MelodicInstrumentRtRefAdaptor(MelodicInstrumentRtRefImpl* pTypeErasedObj) noexcept : m_obj(pTypeErasedObj) {}
   Void noteOn(int note, float velocity, void* token) override
   {
      return m_obj->noteOn(note, velocity, token);
   }
   Void noteOff(int note, float velocity, void* token) override
   {
      return m_obj->noteOff(note, velocity, token);
   }
   void pitchBend(float value) override
   {
      m_obj->pitchBend(value);
   }
   Void pitchBendMPE(int note, float value) override
   {
      return m_obj->pitchBendMPE(note, value);
   }
   Void incrementParameterValue(int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) override
   {
     return m_obj->incrementParameterValue(componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Void incrementParameterValueEventBound(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float increment,
       musicDevice::sound::IncrementMode incrementMode) override
   {
      return m_obj->incrementParameterValueEventBound(componentIdx, parameterId, parameterAttr, increment, incrementMode);
   }
   Ret<float> getParameterValue(
       int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const override
   {
      return m_obj->getParameterValue(componentIdx, parameterIdx, parameterAttr);
   }
   Ret<float> getParameterValue(
       int note, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const override
   {
      return m_obj->getParameterValue(note, componentIdx, parameterIdx, parameterAttr);
   }
   Void setParameterValue(int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) override
   {
     return m_obj->setParameterValue(componentIdx, parameterIdx, parameterAttr, value);
   }
   Ret<float> fromNormalizedValue(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const override
   {
      return m_obj->fromNormalizedValue(note, componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Ret<float> fromNormalizedValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const override
   {
      return m_obj->fromNormalizedValue(componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Void setParameterValueMPE(int note, int componentIdx, int parameterId,
                             musicDevice::sound::ParameterAttr parameterAttr,
                             float value) override
   {
      return m_obj->setParameterValueMPE(note, componentIdx, parameterId, parameterAttr, value);
   }
   Void setRelativeParameterValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue) override
   {
      return m_obj->setRelativeParameterValue(componentIdx, parameterId, parameterAttr, relValue);
   }
private:
   MelodicInstrumentRtRefImpl* m_obj{nullptr};
};
}   // namespace detail
//
class MelodicInstrumentRtRef
{
public:
   // prevent template parameter to be itself
   template <class T>
   requires (!std::is_same_v<std::decay_t<T>, MelodicInstrumentRtRef>)
   MelodicInstrumentRtRef(T& instruments) noexcept 
   {
      static_assert(sizeof(detail::MelodicInstrumentRtRefAdaptor<T>) == sizeof(implBuf_));
      new (&implBuf_) detail::MelodicInstrumentRtRefAdaptor<T>{std::addressof(instruments)};
   }
   Void noteOn(int note, float velocity, void* token = nullptr)
   {
      return getImpl().noteOn(note, velocity, token);
   }
   Void noteOff(int note, float velocity, void* token = nullptr)
   {
      return getImpl().noteOff(note, velocity, token);
   }
   void pitchBend(float value)
   {
      return getImpl().pitchBend(value);
   }
   Void pitchBendMPE(int note, float value)
   {
      return getImpl().pitchBendMPE(note, value);
   }
   Void incrementParameterValue(int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode)
   {
      return getImpl().incrementParameterValue(componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Void incrementParameterValueEventBound(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float increment,
       musicDevice::sound::IncrementMode incrementMode)
   {
      return getImpl().incrementParameterValueEventBound(componentIdx, parameterId, parameterAttr, increment, incrementMode);
   }
   Ret<float> getParameterValue(
       int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const
   {
      return getImpl().getParameterValue(componentIdx, parameterIdx, parameterAttr);
   }
   Ret<float> getParameterValue(
       int note, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const
   {
      return getImpl().getParameterValue(note, componentIdx, parameterIdx, parameterAttr);
   }
   Void setParameterValue(int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value)
   {
      return getImpl().setParameterValue(componentIdx, parameterIdx, parameterAttr, value);
   }
   Ret<float> fromNormalizedValue(
       int note, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const
   {
      return getImpl().fromNormalizedValue(note, componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Ret<float> fromNormalizedValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const
   {
      return getImpl().fromNormalizedValue(componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Void setParameterValueMPE(int note, int componentIdx, int parameterId,
                             musicDevice::sound::ParameterAttr parameterAttr,
                             float value)
   {
      return getImpl().setParameterValueMPE(note, componentIdx, parameterId, parameterAttr, value);
   }
   Void setRelativeParameterValue(
       int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue)
   {
      return getImpl().setRelativeParameterValue(componentIdx, parameterId, parameterAttr, relValue);
   }
private:
   alignas(2 * sizeof(void*))
      std::array<std::byte, 2 * sizeof(void*) > implBuf_;
   detail::MelodicInstrumentRtRefConcept& getImpl() noexcept
   {
      return *reinterpret_cast<detail::MelodicInstrumentRtRefConcept*>(implBuf_.data());
   }
   const detail::MelodicInstrumentRtRefConcept& getImpl() const noexcept
   {
      return *reinterpret_cast<const detail::MelodicInstrumentRtRefConcept*>(implBuf_.data());
   }
};

}   // namespace base::instruments

#endif

