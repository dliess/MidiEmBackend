add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "ControllerHandler.cpp"
)

add_library(MusicDeviceController STATIC ${SOURCES})
target_include_directories(MusicDeviceController PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(MusicDeviceController
   PRIVATE spdlog::spdlog
   PRIVATE MusicDeviceDescription
   PUBLIC Util
   PUBLIC MIDI::midi
   PUBLIC MusicDevicePluginHandler
)