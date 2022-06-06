if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

#TODO: add further source files
set(SOURCES
   "SoundHandler.cpp"
   "DevicePresets.cpp"
   "SoundPresetHandler.cpp"
   "SoundMidiInSysExDumpHandler.cpp"
   "PresetFetcher.cpp"
)

set(HEADERS
   "ParameterPart.h"
   "ParameterCoordinate.h"
   "ParameterScene.h"
   "ParameterSceneContainer.h"
)

set(TARGET_NAME "MusicDeviceSound")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PRIVATE MusicDeviceDescription
   PRIVATE spdlog::spdlog
   PUBLIC Util
   PUBLIC Tempo
   PUBLIC MusicDeviceCommon
   PUBLIC base-arpeggiator
)