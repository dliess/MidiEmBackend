#ifndef INSTRUMENTS_REF_H
#define INSTRUMENTS_REF_H

#include "Identifiable.h"
#include <cstddef>

namespace base::instruments
{

namespace detail
{
class InstrumentsRefConcept
{
public:
   virtual ~InstrumentsRefConcept() = default;
   virtual void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid) = 0;
   virtual void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid) = 0;
   virtual void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid) = 0;
   virtual void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid) = 0;
};

template <class InstrumentsRefImpl>
class InstrumentsRefAdaptor : public InstrumentsRefConcept
{
public:
   InstrumentsRefAdaptor(InstrumentsRefImpl* pTypeErasedObj) noexcept : m_obj(pTypeErasedObj) {}
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid) override
   {
      m_obj->incKitInstrumentRefCount(uuid);
   }
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid) override
   {
      m_obj->decKitInstrumentRefCount(uuid);
   }
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid) override
   {
      m_obj->incMelodicInstrumentRefCount(uuid);
   }
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid) override
   {
      m_obj->decMelodicInstrumentRefCount(uuid);
   }

private:
   InstrumentsRefImpl* m_obj{nullptr};
};
}   // namespace detail
//
class InstrumentsRef
{
public:
   template <class T>
   InstrumentsRef(T& instruments) noexcept 
   {
      static_assert(sizeof(detail::InstrumentsRefAdaptor<T>) == sizeof(implBuf));
      new (&implBuf) detail::InstrumentsRefAdaptor<T>{std::addressof(instruments)};
   }
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      getImpl().incKitInstrumentRefCount(uuid);
   }
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      getImpl().decKitInstrumentRefCount(uuid);
   }
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      getImpl().incMelodicInstrumentRefCount(uuid);
   }
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      getImpl().decMelodicInstrumentRefCount(uuid);
   }

private:
   alignas(2 * sizeof(void*))
      std::array<std::byte, 2 * sizeof(void*) > implBuf;
   detail::InstrumentsRefConcept& getImpl() noexcept
   {
      return *reinterpret_cast<detail::InstrumentsRefConcept*>(implBuf.data());
   }
};

/*
class InstrumentsRef
{
public:
   template <class Instruments>
   requires(!std::is_same_v<std::decay_t<Instruments>, InstrumentsRef>)
       InstrumentsRef(Instruments& instruments) :
       m_obj(std::addressof(instruments)),
       m_vtable(
           {[](void* obj, util::Identifiable::UUIDView uuid,
               util::function_ref<void(const Instrument&)> cb) {
               static_cast<Instruments*>(obj)->withInstrumentRt(uuid, cb);
            },
            [](void* obj, util::Identifiable::UUIDView uuid,
               util::function_ref<void(const KitInstrument&)> cb) {
               static_cast<Instruments*>(obj)->withKitInstrumentRt(uuid, cb);
            },
            [](void* obj, util::Identifiable::UUIDView uuid,
               util::function_ref<void(const MelodicInstrument&)> cb) {
               static_cast<Instruments*>(obj)->withMelodicInstrumentRt(uuid,
                                                                       cb);
            },
            [](void* obj, const util::Identifiable::UUID& uuid){
               return static_cast<Instruments*>(obj)->incKitInstrumentRefCount(uuid);
            },
            [](void* obj, const util::Identifiable::UUID& uuid){
               return static_cast<Instruments*>(obj)->decKitInstrumentRefCount(uuid);
            },
            [](void* obj, const util::Identifiable::UUID& uuid){
               return static_cast<Instruments*>(obj)->incMelodicInstrumentRefCount(uuid);
            },
            [](void* obj, const util::Identifiable::UUID& uuid){
               return static_cast<Instruments*>(obj)->decMelodicInstrumentRefCount(uuid);
            }
            })
   {
   }
   void withInstrumentRt(util::Identifiable::UUIDView uuid,
                         util::function_ref<void(const Instrument&)> cb)
   {
      m_vtable.fn_withInstrumentRt(m_obj, uuid, cb);
   }
   void withKitInstrumentRt(util::Identifiable::UUIDView uuid,
                            util::function_ref<void(const KitInstrument&)> cb)
   {
      m_vtable.fn_withKitInstrumentRt(m_obj, uuid, cb);
   }
   void withMelodicInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const MelodicInstrument&)> cb)
   {
      m_vtable.fn_withMelodicInstrumentRt(m_obj, uuid, cb);
   }
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_incKitInstrumentRefCount(m_obj, uuid);
   }
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_decKitInstrumentRefCount(m_obj, uuid);
   }
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_incMelodicInstrumentRefCount(m_obj, uuid);
   }
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_decMelodicInstrumentRefCount(m_obj, uuid);
   }

private:
   void* m_obj{nullptr};
   struct VTable
   {
      void (*fn_withInstrumentRt)(void* obj, util::Identifiable::UUIDView,
                                  util::function_ref<void(const Instrument&)>) =
          nullptr;
      void (*fn_withKitInstrumentRt)(
          void* obj, util::Identifiable::UUIDView,
          util::function_ref<void(const KitInstrument&)>) = nullptr;
      void (*fn_withMelodicInstrumentRt)(
          void* obj, util::Identifiable::UUIDView,
          util::function_ref<void(const MelodicInstrument&)>) = nullptr;
      void (*fn_incKitInstrumentRefCount)(
          void* obj, const util::Identifiable::UUID& uuid) = nullptr;
      void (*fn_decKitInstrumentRefCount)(
          void* obj, const util::Identifiable::UUID& uuid) = nullptr;
      void (*fn_incMelodicInstrumentRefCount)(
          void* obj, const util::Identifiable::UUID& uuid) = nullptr;
      void (*fn_decMelodicInstrumentRefCount)(
          void* obj, const util::Identifiable::UUID& uuid) = nullptr;
   };
   VTable m_vtable;
};
*/
}   // namespace base::instruments

#endif
