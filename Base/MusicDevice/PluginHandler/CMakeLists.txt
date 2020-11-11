cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "MusicDevicePluginHandler.cpp"
)

add_library(MusicDevicePluginHandler STATIC ${SOURCES})
target_include_directories(MusicDevicePluginHandler
   PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(MusicDevicePluginHandler
   PUBLIC MusicDevicePluginInterface
   PRIVATE dl
)