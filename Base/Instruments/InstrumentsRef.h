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
   requires (!std::is_same_v<std::decay_t<Instruments>, InstrumentsRef>)
   InstrumentsRef(Instruments& instruments) :
       m_pTypeErasedObj(std::addressof(instruments)),
       m_vtable({[](void* obj, util::Identifiable::UUIDView uuid,
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
                 }})
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
   void withMelodicInstrumentRt(util::Identifiable::UUIDView uuid,
                              util::function_ref<void(const MelodicInstrument&)> cb)
   {
      m_vtable.fn_withMelodicInstrumentRt(m_pTypeErasedObj, uuid, cb);
   }

private:
   void* m_pTypeErasedObj{nullptr};
   struct VTable
   {
      void (*fn_withInstrumentRt)(void* obj, util::Identifiable::UUIDView,
                                   util::function_ref<void(const Instrument&)>) =
          nullptr;
      void (*fn_withKitInstrumentRt)(void* obj, util::Identifiable::UUIDView,
                                   util::function_ref<void(const KitInstrument&)>) =
          nullptr;
      void (*fn_withMelodicInstrumentRt)(
          void* obj, util::Identifiable::UUIDView,
          util::function_ref<void(const MelodicInstrument&)>) = nullptr;
   };
   VTable m_vtable;
};

}   // namespace base::instruments

#endif