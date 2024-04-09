#include <gtest/gtest.h>

#include "Mock_Instruments.h"
#include "Track.h"
#include "stack_mempool.h"

using namespace base::session;

TEST(TrackTest, instrumentsRef)
{
   base::instruments::mock::MockInstruments instruments;
   util::StackMempool<101024> memoryPool("mempool");
   std::pmr::polymorphic_allocator<std::byte> allocator(&memoryPool.pool());

   Track track("MyAwesomeTrack", allocator);
   util::Identifiable::UUID uuid;
   uuid_generate(uuid.data());
   track.setInstrumentUUID(instruments, uuid);
}
