set(TARGET_NAME "MusicDeviceControllerEventRouter")

set(SOURCES
    "ControllerEventRouterLoader.cpp"
    "ControllerEventRouter.cpp"
    "ControllerEventRouterRt.cpp"
)

set(HEADERS
    "ControllerEventRouter.h"
    "ControllerEventRouterRt.h"
    "ControllerEventRouterLoader.h"
    "ControllerEventRouterLoaderMeta.h"
    "ControllerEventDestination.h"
    "ControllerEventDestinationHash.h"
    "ControllerEventDestination.inl"
    "ControllerEventDestinationLoader.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME}
    PRIVATE Session
    PRIVATE MusicDevice
    PUBLIC MusicDeviceController
    PUBLIC MusicDeviceCommon
    PUBLIC Util
    PUBLIC Instruments
)

if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()