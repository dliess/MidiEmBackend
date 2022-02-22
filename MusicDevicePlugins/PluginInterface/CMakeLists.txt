cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_library(MusicDevicePluginInterface INTERFACE)
target_include_directories(MusicDevicePluginInterface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(MusicDevicePluginInterface
   INTERFACE Midi::midi
)