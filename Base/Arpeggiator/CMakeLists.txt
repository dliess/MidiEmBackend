set(TARGET_NAME "base-arpeggiator")

set(SOURCES "Arpeggiator.cpp")
set(HEADERS "Interface/Arpeggiator.h")

add_library(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Interface"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${TARGET_NAME} 
    PUBLIC Util)