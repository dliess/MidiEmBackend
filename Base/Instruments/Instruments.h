#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <functional>
#include <vector>
#include "CallbackSignal.h"
#include "DoubleBuffer.h"
#include "Identifiable.h"
#include "Loader/InstrumentsPersister.h"
#include "Loader/InstrumentsData.h"
#include "Loader/KitInstrument.h"
#include "Loader/MelodicInstrument.h"
#include "Rt/InstrumentsData.h"
#include "MusicDevice.h"
// #include "Meta.h"
// #include "Settings_old.h"
#include "function_ref.h"
#include "farbot/AsyncCaller.hpp"
#include "functionMv.h"
#include "Refs/InstrumentRtRef.h"
#include "Refs/KitInstrumentRtRef.h"
#include "Refs/MelodicInstrumentRtRef.h"
// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
namespace base::musicDevice { class MusicDeviceContainer; }
// clang-format on

namespace base::instruments
{
struct Instruments   //: public utils::Settings<Instruments>
{
   explicit Instruments(
       musicDevice::factory::DataHolder& rFactoryDataHolder,
       musicDevice::MusicDeviceContainer& rMDContainer) noexcept;

   // -----------------
   // NON-rt methods:
   // -----------------
   void reEmitSignals();
   void createKitInstrument(std::string name);
   enum class DoEmitChanged { Yes, No };
   void insertKitInstrument(loader::KitInstrument& kitInstrument,
                            DoEmitChanged doEmitChanged = DoEmitChanged::Yes);
   [[nodiscard]] bool hasSameInstrument(
       const loader::KitInstrument& kitInstrument) const;
   void removeKitInstrument(const util::Identifiable::UUID& instrumentId);
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            const std::string& name);

   void createMelodicInstrument(std::string name);
   [[nodiscard]] bool hasSameInstrument(
       const loader::MelodicInstrument& melodicInstrument) const;
   void insertMelodicInstrument(loader::MelodicInstrument& melodicInstrument, 
                                DoEmitChanged doEmitChanged = DoEmitChanged::Yes);
   void removeMelodicInstrument(const util::Identifiable::UUID& instrumentId);
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                const std::string& name);

   Void createNewVoiceInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx);

   Void addComponentToMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx);
   void removeComponentFromMelodicInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx);
   void removeVoiceFromMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx);
   void setNoteOffsetInMelodicInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid,
       int componentIdx, int noteOffset);

   void createNewVoiceInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx);
   void addComponentToKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx);
   void moveKitInstrumentComponent(
       const util::Identifiable::UUID& srcInstrumentUuid, int srcVoiceIdx,
       int srcComponentIdx, const util::Identifiable::UUID& dstInstrumentUuid,
       int dstVoiceIdx);
   void removeComponentFromKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx);
   void removeVoiceFromKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx);
   void setNoteOffsetInKitInstrumentComponent(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int componentIdx, int noteOffset);
   void setNoteOffsetInKitInstrumentVoice(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       int noteOffset);
   void setVoiceNameInKitInstrument(
       const util::Identifiable::UUID& instrumentUuid, int voiceIdx,
       const std::string& name);
   void incKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decKitInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void incMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);
   void decMelodicInstrumentRefCount(const util::Identifiable::UUID& uuid);

   std::string serializeKitInstruments() const;
   std::string serializeMelodicInstruments() const;

   /*
   void withInstrumentRt(util::Identifiable::UUIDView uuid,
                         util::function_ref<void(const rt::Instrument&)> cb);
   void withKitInstrumentRt(util::Identifiable::UUIDView uuid,
                            util::function_ref<void(const rt::KitInstrument&)> cb);
   void withMelodicInstrumentRt(
       util::Identifiable::UUIDView uuid,
       util::function_ref<void(const rt::MelodicInstrument&)> cb);
*/
   void removeReferencesToMD(musicDevice::MusicDevice* pMusicDevice);

   
   [[nodiscard]] bool hasKitInstrument(util::Identifiable::UUIDView uuid) const;
   [[nodiscard]] bool hasMelodicInstrument(
       util::Identifiable::UUIDView uuid) const;
   

   // TODO: maybe move these to rt
   void setKitComponentAmp(util::Identifiable::UUIDView uuid, int voiceIdx,
                           int componentIdx, float amp);
   void setKitVoiceAmp(util::Identifiable::UUIDView uuid, int voiceIdx, float amp);
   void setMelodicComponentAmp(util::Identifiable::UUIDView uuid,
                               int componentIdx, float amp);

   // rt? or loader ?
   void updateParameterUI();

   CB_SIGNAL(DataChanged, const loader::Data&);
   CB_SIGNAL(KitInstrumentParamChanged,
             util::Identifiable::UUIDView, int, int, int,
             musicDevice::sound::ParameterAttr, float);
   CB_SIGNAL(MelodicInstrumentParamChanged,
             util::Identifiable::UUIDView, int, int,
             musicDevice::sound::ParameterAttr, float);
   CB_SIGNAL(KitComponentNoteOffsetChanged, util::Identifiable::UUIDView, int, int, int);
   CB_SIGNAL(KitVoiceNoteOffsetChanged, util::Identifiable::UUIDView, int, int);
   CB_SIGNAL(KitComponentAmpChanged, util::Identifiable::UUIDView, int, int, float);
   CB_SIGNAL(KitVoiceAmpChanged, util::Identifiable::UUIDView, int, float);
   CB_SIGNAL(MelodicComponentNoteOffsetChanged, util::Identifiable::UUIDView, int, int);
   CB_SIGNAL(MelodicComponentAmpChanged, util::Identifiable::UUIDView, int, float);

   Ret<InstrumentRtRef> getInstrumentRtRef(util::Identifiable::UUIDView uuid);
   Ret<KitInstrumentRtRef> getKitInstrumentRtRef(util::Identifiable::UUIDView uuid);
   Ret<MelodicInstrumentRtRef> getMelodicInstrumentRtRef(util::Identifiable::UUIDView uuid);

    void saveIfDirty();

   Ret<const base::musicDevice::description::sound::Parameter*> 
      getParameterDescriptionOfKit(util::Identifiable::UUIDView uuid, int voiceIdx, int componentIdx, int ParameterIdx);
   Ret<const base::musicDevice::description::sound::Parameter*> 
      getParameterDescriptionOfMelodic(util::Identifiable::UUIDView uuid, int componentIdx, int ParameterIdx);
   

   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;
   void invokeQueueActions();
private:
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   base::musicDevice::MusicDeviceContainer& m_rMDContainer;
   rt::Data m_rtData;
   loader::Data m_loaderData;
   loader::Persister m_persister;
   bool m_dirty {false};
   using AsyncCaller = farbot::AsyncCaller<farbot::fifo_options::concurrency::single,
      util::functionMv<120, void()>>; 
   AsyncCaller m_deferToRt;
   AsyncCaller m_deferToLoader;
};

}   // namespace base::instruments

#include "Instruments.inl"

#endif
