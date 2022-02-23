cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(AbletonLinkWrapper)
add_subdirectory(Arpeggiator)
add_subdirectory(Tempo)
add_subdirectory(Instruments)
#add_subdirectory(Mapping)
add_subdirectory(MidiFriends)
add_subdirectory(MusicDevice)
add_subdirectory(Transport)
add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "Base.cpp"
)

set(HEADERS
   "Base.h"
)

add_library(Base STATIC ${SOURCES} ${HEADERS})

target_include_directories(Base
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Base
   PUBLIC  Instruments
   #PUBLIC  Mapping
   PUBLIC  MidiFriends
   PUBLIC  MusicDevice
   PUBLIC  Tempo
   PUBLIC  Transport
   PUBLIC  Util
   PUBLIC  AbletonLinkWrapper
   PRIVATE spdlog::spdlog
   PRIVATE MIDI::usbmidi
   PRIVATE TimeMeasure::TimeMeasure
   PRIVATE capnzero-rt-server
   PRIVATE capnzero-loader-server
)