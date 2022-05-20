add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "BeatTick.cpp"
   "TapTempoHandler.cpp"
)

add_library(Tempo STATIC ${SOURCES})

target_include_directories(Tempo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Tempo
   PRIVATE spdlog::spdlog
   PUBLIC  Util
   PUBLIC  AbletonLinkWrapper
)