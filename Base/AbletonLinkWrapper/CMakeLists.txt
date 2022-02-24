cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

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