set(SOURCES
    "Server.cpp"
    "InstrumentsRpc.cpp"
    "MusicDevicesRpc.cpp"
    "SoundDevicesRpc.cpp"
    "TempoRpc.cpp"
)

set(HEADERS
    "Interface/Server.h"
    "InstrumentsRpc.h"
    "MusicDevicesRpc.h"
    "SoundDevicesRpc.h"
    "TempoRpc.h"
)

set(TARGET_NAME capnzero-server)

add_library(${TARGET_NAME} STATIC 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC midiem-capnzero-rt
    PRIVATE Instruments
    PRIVATE MusicDevice
)