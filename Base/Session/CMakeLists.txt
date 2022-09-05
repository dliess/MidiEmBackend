set(TARGET_NAME "Session")

set(SOURCES
    "Track.cpp"
)

set(HEADERS
    "Track.h"
    "Clip.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${TARGET_NAME}
    PRIVATE Instruments
    PUBLIC Sequencer
    PUBLIC Util 
)