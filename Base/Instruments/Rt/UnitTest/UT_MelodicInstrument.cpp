#include <gtest/gtest.h>

#include "MelodicInstrument.h"
#include "MelodicInstrumentsModifier.h"

using namespace base::instruments::loader;

TEST(MelodicInstrumentTest, test)
{
   MelodicInstrument melodicInstrument("A name");
}

TEST(MelodicInstrumentTest, testJsonSerialization)
{
   MelodicInstrument melodicInstrument("A name");

}
