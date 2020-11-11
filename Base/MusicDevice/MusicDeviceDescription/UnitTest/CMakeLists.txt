cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_musicDeviceDescription.cpp"
)

add_executable(MusicDeviceDescriptionTest ${SOURCES})
target_link_libraries(MusicDeviceDescriptionTest
   MusicDeviceDescription
   gtest 
   pthread
   fmt
)

target_compile_definitions(MusicDeviceDescriptionTest PRIVATE -DRESOURCE_ROOT_DIR="${PROJECT_SOURCE_DIR}")