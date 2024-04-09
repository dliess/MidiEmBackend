#ifndef KIT_INSTRUMENT_RT_REF_H
#define KIT_INSTRUMENT_RT_REF_H

#include <array>
#include <cstddef>
#include "ErrorHandling.h"
#include "ParameterData.h"

namespace base::instruments
{

namespace detail
{
class KitInstrumentRtRefConcept
{
public:
   virtual ~KitInstrumentRtRefConcept() = default;
   virtual Void noteOn(int note, float velocity, void* tkn) = 0;
   virtual Void noteOff(int note, float velocity, void* tkn) = 0;
   virtual Void noteOn(int voiceIdx, int note, float velocity, void* tkn) = 0;
   virtual Void noteOff(int voiceIdx, int note, float velocity, void* tkn) = 0;
   virtual Void incrementParameterValue(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) = 0;
   virtual Void incrementParameterValueEventBound(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) = 0;
   virtual Ret<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const = 0;
   virtual Void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) = 0;
   virtual Ret<float> fromNormalizedValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const = 0;
   virtual Void setRelativeParameterValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue) = 0;
};

template <class KitInstrumentRtRefImpl>
class KitInstrumentRtRefAdaptor : public KitInstrumentRtRefConcept
{
public:
   KitInstrumentRtRefAdaptor(KitInstrumentRtRefImpl* pTypeErasedObj) noexcept : m_obj(pTypeErasedObj) {}
   Void noteOn(int note, float velocity, void* tkn) override
   {
      return m_obj->noteOn(note, velocity, tkn);
   }
   Void noteOff(int note, float velocity, void* tkn) override
   {
      return m_obj->noteOff(note, velocity, tkn);
   }
   Void noteOn(int voiceIdx, int note, float velocity, void* tkn) override
   {
      return m_obj->noteOn(voiceIdx, note, velocity, tkn);
   }
   Void noteOff(int voiceIdx, int note, float velocity, void* tkn) override
   {
      return m_obj->noteOff(voiceIdx, note, velocity, tkn);
   }
   Void incrementParameterValue(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) override
   {
      return m_obj->incrementParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Void incrementParameterValueEventBound(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode) override
   {
      return m_obj->incrementParameterValueEventBound(voiceIdx, componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Ret<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const override
   {
      return m_obj->getParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr);
   }
   Void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value) override
   {
      return m_obj->setParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr, value);
   }
   Ret<float> fromNormalizedValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const override
   {
      return m_obj->fromNormalizedValue(voiceIdx, componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Void setRelativeParameterValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue) override
   {
      return m_obj->setRelativeParameterValue(voiceIdx, componentIdx, parameterId, parameterAttr, relValue);
   }
private:
   KitInstrumentRtRefImpl* m_obj{nullptr};
};
}   // namespace detail
//
class KitInstrumentRtRef
{
public:
   // prevent template parameter to be itself
   template <class T>
   requires (!std::is_same_v<std::decay_t<T>, KitInstrumentRtRef>)
   KitInstrumentRtRef(T& instruments) noexcept 
   {
      static_assert(sizeof(detail::KitInstrumentRtRefAdaptor<T>) == sizeof(implBuf_));
      new (&implBuf_) detail::KitInstrumentRtRefAdaptor<T>{std::addressof(instruments)};
   }
   Void noteOn(int note, float velocity, void* tkn = nullptr)
   {
      return getImpl().noteOn(note, velocity, tkn);
   }
   Void noteOff(int note, float velocity, void* tkn = nullptr)
   {
      return getImpl().noteOff(note, velocity, tkn);
   }
   Void noteOn(int voiceIdx, int note, float velocity, void* tkn = nullptr)
   {
      return getImpl().noteOn(voiceIdx, note, velocity, tkn);
   }
   Void noteOff(int voiceIdx, int note, float velocity, void* tkn = nullptr)
   {
      return getImpl().noteOff(voiceIdx, note, velocity, tkn);
   }
   Void incrementParameterValue(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode)
   {
      return getImpl().incrementParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Void incrementParameterValueEventBound(int voiceIdx, int componentIdx,
                                int parameterIdx,
                                musicDevice::sound::ParameterAttr parameterAttr,
                                float increment, musicDevice::sound::IncrementMode incrementMode)
   {
      return getImpl().incrementParameterValueEventBound(voiceIdx, componentIdx, parameterIdx, parameterAttr, increment, incrementMode);
   }
   Ret<float> getParameterValue(
       int voiceIdx, int componentIdx, int parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const
   {
      return getImpl().getParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr);
   }
   Void setParameterValue(int voiceIdx, int componentIdx, int parameterIdx,
                          musicDevice::sound::ParameterAttr parameterAttr,
                          float value)
   {
      return getImpl().setParameterValue(voiceIdx, componentIdx, parameterIdx, parameterAttr, value);
   }
   Ret<float> fromNormalizedValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr,
       float percentageValue) const
   {
      return getImpl().fromNormalizedValue(voiceIdx, componentIdx, parameterId, parameterAttr, percentageValue);
   }
   Void setRelativeParameterValue(
       int voiceIdx, int componentIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue)
   {
      return getImpl().setRelativeParameterValue(voiceIdx, componentIdx, parameterId, parameterAttr, relValue);
   }
private:
   alignas(2 * sizeof(void*))
      std::array<std::byte, 2 * sizeof(void*) > implBuf_;
   detail::KitInstrumentRtRefConcept& getImpl() noexcept
   {
      return *reinterpret_cast<detail::KitInstrumentRtRefConcept*>(implBuf_.data());
   }
   const detail::KitInstrumentRtRefConcept& getImpl() const noexcept
   {
      return *reinterpret_cast<const detail::KitInstrumentRtRefConcept*>(implBuf_.data());
   }
};

}   // namespace base::instruments

#endif
