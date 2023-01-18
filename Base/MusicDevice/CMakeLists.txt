if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()
add_subdirectory(Common)
add_subdirectory(Controller)
add_subdirectory(MusicDeviceDescription)
add_subdirectory(PluginHandler)
add_subdirectory(Sound)

set(SOURCES
   "ModifiersApplyer.cpp"
   "MusicDevice.cpp"
   "MusicDeviceLoader.cpp"
   "MusicDeviceFactory.cpp"
   "MusicDeviceSequencer.cpp"
   "MusicDeviceFactoryDataHolder.cpp"
   "MusicDeviceInserter.cpp"
)

set(HEADERS
   "ModifiersApplyer.h"
   "MusicDeviceContainer.h"
   "MusicDeviceContainer.inl"
   "MusicDeviceContainerRef.h"
   "MusicDeviceInserter.h"
)

add_library(MusicDevice STATIC ${SOURCES})

target_include_directories(MusicDevice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MusicDevice
   PUBLIC  MusicDeviceCommon
   PUBLIC  MusicDeviceDescription
   PUBLIC  MusicDeviceController
   PUBLIC  MusicDeviceSound
   PUBLIC  MIDI::midi
   PUBLIC  MusicDevicePluginHandler
   PUBLIC  Tempo
   PRIVATE MIDI::usbmidi
   PRIVATE spdlog::spdlog
)