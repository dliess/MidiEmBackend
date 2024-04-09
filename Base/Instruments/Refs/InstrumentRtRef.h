#ifndef INSTRUMENT_RT_REF_H
#define INSTRUMENT_RT_REF_H

#include "Identifiable.h"
#include <cstddef>
#include "CallbackSignal.h"
#include "ErrorHandling.h"

namespace base::instruments
{

namespace detail
{
class InstrumentRtRefConcept
{
public:
   virtual ~InstrumentRtRefConcept() = default;
   virtual util::Identifiable::UUID uuid() const = 0;
   virtual Void noteOn(int note, float velocity, void* tkn) = 0;
   virtual Void noteOff(int note, float velocity, void* tkn) = 0;
   CB_SIGNAL_INTERFACE(NoteOnPlayed, int, float, void*);
   CB_SIGNAL_INTERFACE(NoteOffPlayed, int, float, void*);
};

template <class InstrumentRtRefImpl>
class InstrumentRtRefAdaptor : public InstrumentRtRefConcept
{
public:
   InstrumentRtRefAdaptor(InstrumentRtRefImpl* pTypeErasedObj) noexcept : m_obj(pTypeErasedObj) {}
   util::Identifiable::UUID uuid() const override
   {
      return m_obj->id();
   }
   Void noteOn(int note, float velocity, void* tkn) override
   {
      return m_obj->noteOn(note, velocity, tkn);
   }
   Void noteOff(int note, float velocity, void* tkn) override
   {
      return m_obj->noteOff(note, velocity, tkn);
   }
   void onNoteOnPlayed(NoteOnPlayedCb cb) override
   {
      m_obj->onNoteOnPlayed(std::move(cb));
   }
   void onNoteOffPlayed(NoteOffPlayedCb cb) override
   {
      m_obj->onNoteOffPlayed(std::move(cb));
   }
private:
   InstrumentRtRefImpl* m_obj{nullptr};
};
}   // namespace detail
//
class InstrumentRtRef
{
public:
   // prevent template parameter to be itself
   template <class T>
   requires (!std::is_same_v<std::decay_t<T>, InstrumentRtRef>)
   InstrumentRtRef(T& instruments) noexcept 
   {
      static_assert(sizeof(detail::InstrumentRtRefAdaptor<T>) == sizeof(implBuf));
      new (&implBuf) detail::InstrumentRtRefAdaptor<T>{std::addressof(instruments)};
   }
   util::Identifiable::UUID uuid() const
   {
      return getImpl().uuid();
   }
   Void noteOn(int note, float velocity, void* tkn)
   {
      return getImpl().noteOn(note, velocity, tkn);
   }
   Void noteOff(int note, float velocity, void* tkn)
   {
      return getImpl().noteOff(note, velocity, tkn);
   }
   void onNoteOnPlayed(detail::InstrumentRtRefConcept::NoteOnPlayedCb cb)
   {
      getImpl().onNoteOnPlayed(std::move(cb));
   }
   void onNoteOffPlayed(detail::InstrumentRtRefConcept::NoteOffPlayedCb cb)
   {
      getImpl().onNoteOffPlayed(std::move(cb));
   }
   bool operator==(const InstrumentRtRef& rhs) const noexcept = default;
   bool operator!=(const InstrumentRtRef& rhs) const noexcept = default;

private:
   alignas(2 * sizeof(void*))
      std::array<std::byte, 2 * sizeof(void*) > implBuf;
   detail::InstrumentRtRefConcept& getImpl() noexcept
   {
      return *reinterpret_cast<detail::InstrumentRtRefConcept*>(implBuf.data());
   }
   const detail::InstrumentRtRefConcept& getImpl() const noexcept
   {
      return *reinterpret_cast<const detail::InstrumentRtRefConcept*>(implBuf.data());
   }
};

}   // namespace base::instruments

#endif
