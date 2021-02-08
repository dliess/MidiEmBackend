set(GENERATED_SRCES_DIR ${CMAKE_CURRENT_BINARY_DIR})
capnzero_generate_cpp(CAPNZERO_GEN_SRCS CAPNZERO_GEN_HDRS "schema/Interface.toml")

add_library(CapnZeroGen STATIC ${CAPNZERO_GEN_SRCS})
target_include_directories(CapnZeroGen ${GENERATED_SRCES_DIR})
target_link_libraries(CapnZeroGen
   PRIVATE cppzmq
   PRIVATE CapnProto::capnp-rpc
)