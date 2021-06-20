set(SOURCES
    "LoaderServer.cpp"
    "LoaderRpc.cpp"
)

set(HEADERS
    "Interface/LoaderServer.h"
    "LoaderRpc.h"
)

set(TARGET_NAME capnzero-loader-server)

add_library(${TARGET_NAME} STATIC 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC midiem-capnzero-loader
    PRIVATE MusicDevice
)