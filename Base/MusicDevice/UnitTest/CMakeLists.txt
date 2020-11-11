cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_MusicDevice.cpp"
)

add_executable(MusicDeviceTest ${SOURCES})
target_link_libraries(MusicDeviceTest
   MusicDevice
   MusicDeviceDescription
   gtest 
   pthread
)