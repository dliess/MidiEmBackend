set(TARGET_NAME "AbletonLinkWrapper")

set(SOURCES
   "AbletonLinkWrapper.cpp"
)

set(HEADERS
   "AbletonLinkWrapper.h"
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME}
   PUBLIC  Util
   PRIVATE Tempo
   PRIVATE Transport
   PRIVATE AbletonLink::AbletonLink
)