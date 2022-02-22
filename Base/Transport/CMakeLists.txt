cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
  "TransportControl.cpp"
)

add_library(Transport STATIC ${SOURCES})

target_include_directories(Transport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Transport
   PUBLIC  MusicDevice
   PRIVATE spdlog::spdlog
   PRIVATE Ableton::Link
   PUBLIC  util
)