add_subdirectory(itc)
if(NOMIDI_BACKEND_BUILD_TESTS)
    add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
    "FilePersister.cpp"
    "FdSet.cpp"
    "FileReader.cpp"
    "HomeDir.cpp"
)

set(HEADERS
    "IDataPersister.h"
    "FilePersister.h"
    "FdSet.h"
    "error_msg.hpp"
    "FloatEqual.h"
    "FixedSizeString.h"
    "function_ref.h"
    "function.h"
    "DoubleBuffer.h"
    "Round1000.h"
    "StrongType.h"
    "toIntegral.h"
    "ErrorHandling.h"
)

set(TARGET_NAME Util)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(${TARGET_NAME}
    PUBLIC MetaStuff::Meta
    PUBLIC MetaStuff::SerializeJson
    PUBLIC spdlog::spdlog
    PUBLIC Variant::Variant
    PUBLIC -lstdc++fs
    PUBLIC uuid
    PUBLIC UtilItc
    PUBLIC tl::expected
)
