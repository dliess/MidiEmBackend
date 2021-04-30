cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(itc)
add_subdirectory(Serialize)
add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
    "FdSet.cpp"
    "FileReader.cpp"
    "HomeDir.cpp"
)

set(HEADERS
    "FdSet.h"
    "error_msg.hpp"
)

add_library(Util STATIC ${SOURCES} ${HEADERS})
target_include_directories(Util
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(Util
    PUBLIC UtilSerialize
    PUBLIC Loguru
    PUBLIC mpark_variant
    PUBLIC -lstdc++fs
    PUBLIC uuid
    PUBLIC UtilItc
)