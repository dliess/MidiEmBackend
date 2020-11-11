cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_PluginHandler.cpp"
)

add_executable(MusicDevicePluginHandlerTest ${SOURCES})
target_link_libraries(MusicDevicePluginHandlerTest
   MusicDevicePluginHandler
   gtest 
   pthread
)