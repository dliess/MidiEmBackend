#add_subdirectory(capnproto)
add_subdirectory(generator)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/generator/cmake")
include(capnzero_generate_cpp)
capnzero_generate_cpp(CAPNZERO_GEN_SRCS CAPNZERO__GEN_HDRS "schema/Interface.toml")

set(SOURCES
    "ZeroMqAdapter.cpp"
)

add_library(ZeroMqAdapter STATIC ${SOURCES} ${CAPNZERO_GEN_SRCS})
target_include_directories(ZeroMqAdapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ZeroMqAdapter
    PUBLIC cppzmq
)
