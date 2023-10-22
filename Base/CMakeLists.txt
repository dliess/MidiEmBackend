add_subdirectory(AbletonLinkWrapper)
add_subdirectory(Arpeggiator)
add_subdirectory(Tempo)
add_subdirectory(Session)
add_subdirectory(Sequencer)
add_subdirectory(Instruments)
#add_subdirectory(Mapping)
add_subdirectory(MidiFriends)
add_subdirectory(MusicDevice)
add_subdirectory(Transport)
add_subdirectory(ControllerEventRouter)
add_subdirectory(ParameterScenes)
if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

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
   PUBLIC  MusicDeviceControllerEventRouter
   PUBLIC  Tempo
   PUBLIC  Transport
   PUBLIC  Util
   PUBLIC  AbletonLinkWrapper
   PUBLIC  Session
   PUBLIC  ParameterScenes
   PRIVATE spdlog::spdlog
   PRIVATE MIDI::usbmidi
   # PRIVATE TimeMeasure::TimeMeasure
   PRIVATE capnzero-rt-server
   PRIVATE capnzero-loader-server
)
