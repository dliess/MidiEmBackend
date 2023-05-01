#ifndef INSTRUMENTS_REF_H
#define INSTRUMENTS_REF_H

#include "Identifiable.h"
#include "function_ref.h"

namespace base::instruments
{
class MelodicInstrument;
class KitInstrument;

class Instrument;

class InstrumentsRef
{
public:
   template <class Instruments>
   requires(!std::is_same_v<std::decay_t<Instruments>, InstrumentsRef>)
       InstrumentsRef(Instruments& instruments) :
       m_pTypeErasedObj(std::addressof(instruments)),
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
            [](void* obj,
               util::Identifiable::UUIDView uuid) -> const Instrument* {
               return static_cast<Instruments*>(obj)->getInstrumentByUuid(uuid);
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
      m_vtable.fn_withInstrumentRt(m_pTypeErasedObj, uuid, cb);
   }
   void withKitInstrumentRt(util::Identifiable::UUIDView uuid,
                            util::function_ref<void(const KitInstrument&)> cb)
   {
      m_vtable.fn_withKitInstrumentRt(m_pTypeErasedObj, uuid, cb);
   }
   void withMelodicInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const MelodicInstrument&)> cb)
   {
      m_vtable.fn_withMelodicInstrumentRt(m_pTypeErasedObj, uuid, cb);
   }
   [[nodiscard]] const Instrument* getInstrumentByUuid(
       util::Identifiable::UUIDView uuid)
   {
      return m_vtable.fn_getInstrumentByUuid(m_pTypeErasedObj, uuid);
   }
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_incKitInstrumentRefCount(m_pTypeErasedObj, uuid);
   }
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_decKitInstrumentRefCount(m_pTypeErasedObj, uuid);
   }
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_incMelodicInstrumentRefCount(m_pTypeErasedObj, uuid);
   }
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid)
   {
      return m_vtable.fn_decMelodicInstrumentRefCount(m_pTypeErasedObj, uuid);
   }

private:
   void* m_pTypeErasedObj{nullptr};
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
      const Instrument* (*fn_getInstrumentByUuid)(
          void* obj, util::Identifiable::UUIDView uuid) = nullptr;
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

}   // namespace base::instruments

#endif