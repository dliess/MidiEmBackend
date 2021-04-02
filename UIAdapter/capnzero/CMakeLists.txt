include(capnzero_generate_cpp)

set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

capnzero_generate_cpp(
    GEN_CPP_SOURCES gen_cpp_sources
    GEN_CPP_HEADERS gen_cpp_headers
    GEN_OUT_DIR ${OUTPUT_DIR}
    IDL_FILE "schema/MidiEm.toml"
)

set(TARGET_NAME "midiem-backend-capnzero")

add_library(${TARGET_NAME} STATIC 
    ${gen_cpp_sources}
    ${gen_cpp_headers}
)
target_include_directories(${TARGET_NAME}
    PUBLIC
        ${OUTPUT_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET_NAME}
    PRIVATE cppzmq
    PRIVATE CapnProto::capnp-rpc
    PUBLIC CAPNZERO::capnzero
)