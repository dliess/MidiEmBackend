find_package(CapnProto CONFIG REQUIRED)


capnzero_generate_cpp(CAPNZERO_SRCS CAPNZERO_HDRS "schema/Interface.toml")


set(CAPNPROTO_FILE "${CMAKE_CURRENT_BINARY_DIR}/Interface.capnp")

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${CAPNPROTO_FILE})

set(TARGET_NAME "uiadapter-capnproto")

add_library(${TARGET_NAME} ${CAPNP_SRCS})
target_link_libraries(${TARGET_NAME} PRIVATE CapnProto::capnp-rpc)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
