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
   "ParameterAttr.h"
   "ParameterCoordinate.h"
   "SoundParameterStorage.h"
   "SoundParameterStorage.inl"
   "SoundParameterStorageElement.h"
   "SoundParameterStorageElement.inl"
   "LFO.h"
   "LFO.inl"
   "LFOData.h"
   "LFODataMeta.h"
   "ParameterData.h"
   "DirtyFlags.h"
   "ValueModifier.h"
)

set(TARGET_NAME "MusicDeviceSound")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PRIVATE MusicDeviceDescriptions::MusicDeviceDescription
   PRIVATE spdlog::spdlog
   PUBLIC Util
   PUBLIC Tempo
   PUBLIC MusicDeviceCommon
   PUBLIC base-arpeggiator
   PUBLIC magic_enum::magic_enum
)