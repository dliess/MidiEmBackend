cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_ControllerHandler.cpp"
   "UT_ControllerMidiInMsgHandler.cpp"
   "UT_ControllerMidiOutMsgHandler.cpp"
)

add_executable(MusicDeviceControllerTest ${SOURCES})
target_link_libraries(MusicDeviceControllerTest
   MusicDeviceController
   MusicDeviceDescription
   gtest 
   pthread
)