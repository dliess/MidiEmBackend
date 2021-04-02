find_package(CapnProto CONFIG REQUIRED)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS schema/schema.capnp)

set(TARGET_NAME "capnproto-test")

set(SOURCES
    "main.cpp"
)

add_executable(${TARGET_NAME} ${SOURCES} ${CAPNP_SRCS})
target_link_libraries(${TARGET_NAME}
    PRIVATE CapnProto::capnp-rpc
    PRIVATE CapnProto::capnp-json
)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
