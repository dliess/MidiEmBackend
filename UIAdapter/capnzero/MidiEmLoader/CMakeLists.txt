set(SOURCES
    "LoaderServer.cpp"
    "LoaderRpc.cpp"
    "RtClient.cpp"
    "LdControllerEventRouterRpc.cpp"
    "InstrumentsRpc.cpp"
)

set(HEADERS
    "Interface/LoaderServer.h"
    "Interface/RtClient.h"
    "LoaderRpc.h"
    "LdControllerEventRouterRpc.h"
    "InstrumentsRpc.h"
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
    PUBLIC MIDIEMCAPNZERO::midiem-capnzero-loader
    PUBLIC MIDIEMCAPNZERO::midiem-capnzero-rt
    PUBLIC MIDI::usbmidi
    PRIVATE MusicDevice
    PUBLIC MusicDeviceControllerEventRouter
)