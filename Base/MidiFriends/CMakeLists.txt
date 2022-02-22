cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "MidiDumper.cpp"
   "MidiRouter.cpp"
)

add_library(MidiFriends STATIC ${SOURCES})

target_include_directories(MidiFriends PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MidiFriends
   PUBLIC  MusicDevice
   PRIVATE spdlog::spdlog
   PUBLIC  util
   PUBLIC  midi
)