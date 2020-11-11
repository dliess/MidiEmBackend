cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_Instruments.cpp"
   "UT_InstrumentsFactory.cpp"
   "UT_KitInstrument.cpp"
   "UT_MelodicInstrument.cpp"
)

add_executable(InstrumentsTest ${SOURCES})
target_link_libraries(InstrumentsTest
   Instruments
   gtest 
   pthread
)