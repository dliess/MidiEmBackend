if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

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