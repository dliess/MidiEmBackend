add_subdirectory(capnproto)

set(SOURCES
    "ZeroMqAdapter.cpp"
)

add_library(ZeroMqAdapter STATIC ${SOURCES})
target_include_directories(ZeroMqAdapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ZeroMqAdapter
    PUBLIC cppzmq
)
