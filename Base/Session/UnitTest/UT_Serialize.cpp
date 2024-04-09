#include <gtest/gtest.h>

#include "Mock_Instruments.h"
#include "Tracks.h"
#include "stack_mempool.h"

TEST(SerializeTest, clip)
{
   util::StackMempool<101024> memoryPool("mempool");
   std::pmr::polymorphic_allocator<std::byte> allocator(&memoryPool.pool());
   base::session::Clip clip(allocator);
   clip.addNote(1.0, 1.0, 64, 1.0);
   clip.addNote(2.0, 1.0, 66, 1.0);
   clip.addNote(3.0, 1.0, 68, 1.0);
   clip.addNote(2.0, 1.0, 70, 1.0);
   nlohmann::json j = clip;
   spdlog::info(j.dump());
}

TEST(SerializeTest, track)
{
   base::instruments::mock::MockInstruments instruments;
   util::StackMempool<101024> memoryPool("mempool");
   std::pmr::polymorphic_allocator<std::byte> allocator(&memoryPool.pool());
   base::session::Track track("TestTrack", allocator);
   track.createClip(4);
   track.clip(4)->addNote(1.0, 1.0, 64, 1.0);
   track.clip(4)->addNote(2.0, 1.0, 66, 1.0);
   track.clip(4)->addNote(3.0, 1.0, 68, 1.0);
   track.clip(4)->addNote(2.0, 1.0, 70, 1.0);
   nlohmann::json j = track;
   spdlog::info(j.dump());
}
