#add_subdirectory(capnproto)

capnzero_generate_cpp(CAPNZERO_GEN_SRCS CAPNZERO__GEN_HDRS "schema/Interface.toml")

set(SOURCES
    "ZeroMqAdapter.cpp"
)

add_library(ZeroMqAdapter STATIC ${SOURCES} ${CAPNZERO_GEN_SRCS})
target_include_directories(ZeroMqAdapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ZeroMqAdapter
    PUBLIC cppzmq
)
