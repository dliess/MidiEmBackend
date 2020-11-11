cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_BeatTick.cpp"
   "UT_TapTempoHandler.cpp"
)

add_executable(TempoTest ${SOURCES})
target_link_libraries(TempoTest
   Tempo
   gtest 
   pthread
)