add_subdirectory(itc)
add_subdirectory(Serialize)
if(NOMIDI_BACKEND_BUILD_TESTS)
    add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
    "FdSet.cpp"
    "FileReader.cpp"
    "HomeDir.cpp"
)

set(HEADERS
    "FdSet.h"
    "error_msg.hpp"
    "FloatEqual.h"
    "function_ref.h"
)

set(TARGET_NAME Util)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(${TARGET_NAME}
    PUBLIC UtilSerialize
    PUBLIC spdlog::spdlog
    PUBLIC mpark_variant
    PUBLIC -lstdc++fs
    PUBLIC uuid
    PUBLIC UtilItc
)