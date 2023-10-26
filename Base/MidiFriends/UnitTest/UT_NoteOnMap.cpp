#include <gtest/gtest.h>
#include <stdexcept>

#include "NoteOnMap.h"

using namespace base::midifriends;

TEST(NoteOnMap, setNoteOn)
{
   NoteOnMap map;
   map.setNoteOn(0, 0);
   map.setNoteOn(0, 42);
   EXPECT_THROW(map.setNoteOn(100, 42), std::out_of_range);
   EXPECT_THROW(map.setNoteOn(0, 190), std::out_of_range);
}


TEST(NoteOnMap, setNoteOff)
{
   NoteOnMap map;
   map.setNoteOff(0, 0);
   map.setNoteOff(0, 42);
   EXPECT_THROW(map.setNoteOff(100, 42), std::out_of_range);
   EXPECT_THROW(map.setNoteOff(0, 190), std::out_of_range);
}

TEST(NoteOnMap, forEachNoteOn)
{
   NoteOnMap map;
   map.setNoteOn(3, 42);

   map.forEachNoteOn([](int channelIdx, int note) {
      EXPECT_EQ(channelIdx, 3);
      EXPECT_EQ(note, 42);
   });
}

TEST(NoteOnMap, clear)
{
   NoteOnMap map;
   map.setNoteOn(0, 0);
   map.setNoteOn(0, 42);

   map.clear();

   map.forEachNoteOn([](int channelIdx, int note) {
      FAIL();
   });
}
