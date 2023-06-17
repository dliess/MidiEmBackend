if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(HEADERS
   "ControllerAdditionalEventCreator.h"
)

set(SOURCES
   "ControllerHandler.cpp"
   "ControllerAdditionalEventCreator.cpp"
)

add_library(MusicDeviceController STATIC ${SOURCES} ${HEADERS})
target_include_directories(MusicDeviceController PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(MusicDeviceController
   PRIVATE spdlog::spdlog
   PRIVATE MusicDeviceDescription
   PUBLIC Util
   PUBLIC MIDI::midi
   PUBLIC MusicDevicePluginHandler
)