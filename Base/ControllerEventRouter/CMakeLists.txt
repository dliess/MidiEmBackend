set(TARGET_NAME "MusicDeviceControllerEventRouter")

set(SOURCES
    "ControllerEventRouter.cpp"    
)

set(HEADERS
    "ControllerEventRouter.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME}
    PRIVATE MusicDevice
    PUBLIC Util
)