set(TARGET_NAME "Session")

set(SOURCES
    "Track.cpp"
    "Tracks.cpp"
    "Clip.cpp"
)

set(HEADERS
    "Track.h"
    "Track.inl"
    "Tracks.h"
    "Tracks.inl"
    "Clip.h"
    "Clip.inl"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
    PRIVATE Instruments
    PRIVATE Tempo
    PUBLIC Sequencer
    PUBLIC Util
)