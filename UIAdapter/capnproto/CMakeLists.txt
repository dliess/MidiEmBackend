find_package(CapnProto CONFIG REQUIRED)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS schema/schema.capnp)

set(TARGET_NAME "uiadapter-capnproto")

add_library(${TARGET_NAME} ${CAPNP_SRCS})
target_link_libraries(${TARGET_NAME} PRIVATE CapnProto::capnp-rpc)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
