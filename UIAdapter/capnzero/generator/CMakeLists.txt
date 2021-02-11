add_library(capnzero INTERFACE)

target_include_directories(capnzero INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(capnzero INTERFACE span)