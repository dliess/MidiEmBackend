if(NOMIDI_BACKEND_BUILD_TESTS)
  add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
  "TransportControl.cpp"
)

add_library(Transport STATIC ${SOURCES})

target_include_directories(Transport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Transport
   PUBLIC  MusicDevice
   PRIVATE spdlog::spdlog
   PRIVATE AbletonLink::AbletonLink
   PUBLIC  util
)