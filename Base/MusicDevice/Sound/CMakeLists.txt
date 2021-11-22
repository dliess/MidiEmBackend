cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

#TODO: add further source files
set(SOURCES
   "SoundHandler.cpp"
   "DevicePresets.cpp"
   "SoundPresetHandler.cpp"
   "SoundMidiInSysExDumpHandler.cpp"
   "PresetFetcher.cpp"
)

add_library(MusicDeviceSound STATIC ${SOURCES})

target_include_directories(MusicDeviceSound PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MusicDeviceSound
   PRIVATE MusicDeviceDescription
   PRIVATE Loguru
   PUBLIC Util
   PUBLIC Tempo
   PUBLIC MusicDeviceCommon
   PUBLIC base-arpeggiator
)