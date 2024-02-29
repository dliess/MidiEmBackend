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
// #include "Meta.h"
// #include "Settings_old.h"
#include "function_ref.h"
#include "farbot/AsyncCaller.hpp"
#include "functionMv.h"
// clang-format off
namespace base::musicDevice::factory { class DataHolder; }
// clang-format on

namespace base::instruments
{
struct Instruments   //: public utils::Settings<Instruments>
{
   explicit Instruments(
       musicDevice::factory::DataHolder& rFactoryDataHolder) noexcept;

   // -----------------
   // NON-rt methods:
   // -----------------
   void reEmitSignals();
   void createKitInstrument(std::string name);
   void insertKitInstrument(loader::KitInstrument& kitInstrument);
   [[nodiscard]] bool hasSameInstrument(
       const loader::KitInstrument& kitInstrument) const;
   void removeKitInstrument(const util::Identifiable::UUID& instrumentId);
   void renameKitInstrument(const util::Identifiable::UUID& instrumentId,
                            const std::string& name);

   void createMelodicInstrument(std::string name);
   [[nodiscard]] bool hasSameInstrument(
       const loader::MelodicInstrument& melodicInstrument) const;
   void insertMelodicInstrument(loader::MelodicInstrument& melodicInstrument);
   void removeMelodicInstrument(const util::Identifiable::UUID& instrumentId);
   void renameMelodicInstrument(const util::Identifiable::UUID& instrumentId,
                                const std::string& name);

   void createNewVoiceInMelodicInstrument(
       const util::Identifiable::UUID& instrumentUuid,
       const util::Identifiable::UUID& soundDeviceUuid, int sdVoiceIdx);

   void addComponentToMelodicInstrumentVoice(
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
   void fillReferencesToMD(musicDevice::MusicDevice* pMusicDevice);
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

   CB_SIGNAL(DataChanged, const loader::Data&, bool);
   CB_SIGNAL_SINGLE_SUBSCRIBER(KitInstrumentParamChanged,
                               util::Identifiable::UUIDView, int, int, int,
                               musicDevice::sound::ParameterAttr, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(MelodicInstrumentParamChanged,
                               util::Identifiable::UUIDView, int, int,
                               musicDevice::sound::ParameterAttr, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(KitComponentNoteOffsetChanged, util::Identifiable::UUIDView, int, int, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(KitVoiceNoteOffsetChanged, util::Identifiable::UUIDView, int, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(KitComponentAmpChanged, util::Identifiable::UUIDView, int, int, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(KitVoiceAmpChanged, util::Identifiable::UUIDView, int, float);
   CB_SIGNAL_SINGLE_SUBSCRIBER(MelodicComponentNoteOffsetChanged, util::Identifiable::UUIDView, int, int);
   CB_SIGNAL_SINGLE_SUBSCRIBER(MelodicComponentAmpChanged, util::Identifiable::UUIDView, int, float);

   // loop it back here to Loader
   void kitParamChanged(const util::Identifiable::UUID& uuid, int voiceIdx,
                        int componentIdx, int parameterIdx,
                        musicDevice::sound::ParameterAttr parameterAttr,
                        float value);

   void melodicParamChanged(const util::Identifiable::UUID& uuid,
                            int componentIdx, int parameterIdx,
                            musicDevice::sound::ParameterAttr parameterAttr,
                            float value);

    void saveIfDirty();

   friend class MelodicInstrumentsParameterCacheCreator;
   friend class KitInstrumentsParameterCacheCreator;
   void invokeQueueActions();
private:
   base::musicDevice::factory::DataHolder& m_rFactoryDataHolder;
   rt::Data m_rtData;
   loader::Data m_loaderData;
   loader::Persister m_persister;
   bool m_parameterCacheDirty {false};
   using AsyncCaller = farbot::AsyncCaller<farbot::fifo_options::concurrency::single,
      util::functionMv<120, void()>>; 
   AsyncCaller m_deferToRt;
   AsyncCaller m_deferToLoader;
};

}   // namespace base::instruments

#include "Instruments.inl"

#endif
