add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

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