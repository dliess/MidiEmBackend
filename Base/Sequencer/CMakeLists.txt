set(TARGET_NAME "Sequencer")

set(SOURCES
    ""
)

set(HEADERS
    "NoteEvent.h"
    "TimedEvent.h"
)

add_library(${TARGET_NAME} INTERFACE ${HEADERS})