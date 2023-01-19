#ifndef MOCK_INSTRUMENTS_H
#define MOCK_INSTRUMENTS_H

#include "gmock/gmock.h"
#include "Identifiable.h"
#include "function_ref.h"
#include "KitInstrument.h"
#include "MelodicInstrument.h"

namespace base::instruments::mock
{
class MockInstruments
{
public:
   MOCK_METHOD(void, withKitInstrument, (util::Identifiable::UUIDView, util::function_ref<void(const KitInstrument&)>));
   MOCK_METHOD(void, withMelodicInstrument, (util::Identifiable::UUIDView, util::function_ref<void(const MelodicInstrument&)>));
};

}   // namespace mock

#endif