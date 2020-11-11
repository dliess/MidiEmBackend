cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_MidiDumper.cpp"
   "UT_MidiRouter.cpp"
)

add_executable(MidiFriendsTest ${SOURCES})
target_link_libraries(MidiFriendsTest
   MidiFriends
   gtest 
   pthread
)