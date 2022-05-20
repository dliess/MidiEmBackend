add_library(MusicDevicePluginInterface INTERFACE)
target_include_directories(MusicDevicePluginInterface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(MusicDevicePluginInterface
   INTERFACE MIDI::midi
)