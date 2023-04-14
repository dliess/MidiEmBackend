#include <gtest/gtest.h>

#include "Instruments.h"
#include "InstrumentsRef.h"
#include "MusicDeviceFactoryDataHolder.h"

using namespace base::musicDevice;
using namespace base::instruments;

TEST(InstrumentsTest, test)
{
   factory::DataHolder factoryDataHolder("~");
   Instruments instruments(factoryDataHolder);
   InstrumentsRef instrumentsRef(instruments);
   instrumentsRef.withInstrumentRt(
       util::Identifiable::UUID{},
       [](const auto& instr) { instr.refCount(); });
    InstrumentsRef instrumentsRef2(instrumentsRef);
   instrumentsRef2.withInstrumentRt(
       util::Identifiable::UUID{},
       [](const auto& instr) { instr.refCount(); });
}