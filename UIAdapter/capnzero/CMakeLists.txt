set(SOURCES
    "Server.cpp"
    "MusicDevicesRpc.cpp"
    "InstrumentsRpc.cpp"
)

set(HEADERS
    "Server.h"
    "MusicDevicesRpc.h"
    "InstrumentsRpc.h"
)

set(TARGET_NAME capnzero-server)

add_library(${TARGET_NAME} STATIC 
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC 
        midiem-capnzero
)