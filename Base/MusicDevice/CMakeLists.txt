cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
add_subdirectory(Common)
add_subdirectory(Controller)
add_subdirectory(MusicDeviceDescription)
add_subdirectory(PluginHandler)
add_subdirectory(Sound)

set(SOURCES
   "MusicDevice.cpp"
   "MusicDeviceLoader.cpp"
   "MusicDeviceFactory.cpp"
   "MusicDeviceSequencer.cpp"
   "MusicDeviceFactoryDataHolder.cpp"
)

add_library(MusicDevice STATIC ${SOURCES})

target_include_directories(MusicDevice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MusicDevice
   PUBLIC  MusicDeviceCommon
   PUBLIC  MusicDeviceDescription
   PUBLIC  MusicDeviceController
   PUBLIC  MusicDeviceSound
   PUBLIC  midi
   PUBLIC  usbMidiPortHandling
   PUBLIC  MusicDevicePluginHandler
   PUBLIC  Tempo
   PRIVATE usbmidi
   PRIVATE Loguru
)

option(__INSERT_DUMMY_MIDI_DEVICES__ "insert dummy devices for testing" OFF)
if(${__INSERT_DUMMY_MIDI_DEVICES__})
   target_compile_definitions(MusicDevice PUBLIC "__INSERT_DUMMY_MIDI_DEVICES__")
   target_link_libraries(MusicDevice PRIVATE MidiMediumDummy)
endif(${__INSERT_DUMMY_MIDI_DEVICES__})
