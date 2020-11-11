cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

add_library(UtilItc INTERFACE)
target_include_directories(UtilItc
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)