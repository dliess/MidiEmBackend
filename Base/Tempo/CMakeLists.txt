cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "BeatTick.cpp"
   "TapTempoHandler.cpp"
)

add_library(Tempo STATIC ${SOURCES})

target_include_directories(Tempo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Tempo
   PRIVATE Loguru
   PUBLIC  Util
)