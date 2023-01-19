#ifndef INSTRUMENTS_REF_H
#define INSTRUMENTS_REF_H

#include "Identifiable.h"
#include "function_ref.h"

namespace base::instruments
{
class MelodicInstrument;
class KitInstrument;

class InstrumentsRef
{
public:
   template <class Instruments>
   InstrumentsRef(Instruments& instruments) :
       m_pTypeErasedObj(std::addressof(instruments)),
       m_vtable({[](void* obj, util::Identifiable::UUIDView uuid,
                    util::function_ref<void(const KitInstrument&)> cb) {
                    static_cast<Instruments*>(obj)->withKitInstrument(uuid, cb);
                 },
                 [](void* obj, util::Identifiable::UUIDView uuid,
                    util::function_ref<void(const MelodicInstrument&)> cb) {
                    static_cast<Instruments*>(obj)->withMelodicInstrument(uuid,
                                                                          cb);
                 }})
   {
   }
   void withKitInstrument(util::Identifiable::UUIDView uuid,
                          util::function_ref<void(const KitInstrument&)> cb)
   {
      m_vtable.fn_withKitInstrument(m_pTypeErasedObj, uuid, cb);
   }
   void withMelodicInstrument(util::Identifiable::UUIDView uuid,
                              util::function_ref<void(const MelodicInstrument&)> cb)
   {
      m_vtable.fn_withMelodicInstrument(m_pTypeErasedObj, uuid, cb);
   }

private:
   void* m_pTypeErasedObj{nullptr};
   struct VTable
   {
      void (*fn_withKitInstrument)(void* obj, util::Identifiable::UUIDView,
                                   util::function_ref<void(KitInstrument&)>) =
          nullptr;
      void (*fn_withMelodicInstrument)(
          void* obj, util::Identifiable::UUIDView,
          util::function_ref<void(MelodicInstrument&)>) = nullptr;
   };
   VTable m_vtable;
};

}   // namespace base::instruments

#endif