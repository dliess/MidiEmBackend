set(SOURCES
    "AbletonLinkRpc.cpp"
    "MainRpc.cpp"
    "RtServer.cpp"
    "InstrumentsRpc.cpp"
    "MidiRoutingRpc.cpp"
    "SoundDevicesRpc.cpp"
    "TempoRpc.cpp"
    "TransportControlRpc.cpp"
    "ControllerDevicesRpc.cpp"
    "ControllerEventRouterRpc.cpp"
    "ParameterSceneRpc.cpp"
    "SessionRpc.cpp"
)

set(HEADERS
    "Interface/RtServer.h"
    "AbletonLinkRpc.h"
    "MainRpc.h"
    "InstrumentsRpc.h"
    "MidiRoutingRpc.h"
    "SoundDevicesRpc.h"
    "TempoRpc.h"
    "TransportControlRpc.h"
    "ControllerDevicesRpc.h"
    "ControllerEventRouterRpc.h"
    "ParameterSceneRpc.h"
    "SessionRpc.h"
)

set(TARGET_NAME capnzero-rt-server)

add_library(${TARGET_NAME} STATIC 
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME} 
    PUBLIC MIDIEMCAPNZERO::midiem-capnzero-rt
    PRIVATE Instruments
    PRIVATE MusicDevice
    PRIVATE Transport
    PRIVATE MidiFriends
    PRIVATE AbletonLinkWrapper
    PRIVATE MusicDeviceControllerEventRouter
    PRIVATE Session
)