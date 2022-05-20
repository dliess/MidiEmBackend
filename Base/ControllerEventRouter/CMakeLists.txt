set(TARGET_NAME "MusicDeviceControllerEventRouter")

set(SOURCES
    "ControllerEventRouter.cpp"    
)

set(HEADERS
    "ControllerEventRouter.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET_NAME} PRIVATE MusicDevice)