#ifndef MOCK_INSTRUMENTS_H
#define MOCK_INSTRUMENTS_H

#include "gmock/gmock.h"
#include "Identifiable.h"
#include "Refs/InstrumentRtRef.h"
#include "Refs/KitInstrumentRtRef.h"
#include "Refs/MelodicInstrumentRtRef.h"
#include "SoundSection.h"
#include "ErrorHandling.h"

namespace base::instruments::mock
{
class MockInstruments
{
public:
   MOCK_METHOD(void, incKitInstrumentRefCount, (const util::Identifiable::UUID&));
   MOCK_METHOD(void, decKitInstrumentRefCount, (const util::Identifiable::UUID&));
   MOCK_METHOD(void, incMelodicInstrumentRefCount, (const util::Identifiable::UUID&));
   MOCK_METHOD(void, decMelodicInstrumentRefCount, (const util::Identifiable::UUID&));
   MOCK_METHOD(Ret<InstrumentRtRef>, getInstrumentRtRef, (util::Identifiable::UUIDView));
   MOCK_METHOD(Ret<KitInstrumentRtRef>, getKitInstrumentRtRef, (util::Identifiable::UUIDView));
   MOCK_METHOD(Ret<MelodicInstrumentRtRef>, getMelodicInstrumentRtRef, (util::Identifiable::UUIDView));
   MOCK_METHOD(Ret<const base::musicDevice::description::sound::Parameter*>, getParameterDescriptionOfKit, (util::Identifiable::UUIDView, int, int, int));
   MOCK_METHOD(Ret<const base::musicDevice::description::sound::Parameter*>, getParameterDescriptionOfMelodic, (util::Identifiable::UUIDView, int, int));
};

}   // namespace mock

#endif
