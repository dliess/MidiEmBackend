set(TARGET_NAME "Sequencer")

set(SOURCES
    ""
)

set(HEADERS
    "NoteEvent.h"
    "TimedEvent.h"
)

add_library(${TARGET_NAME} INTERFACE)

target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})