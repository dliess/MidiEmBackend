cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_TransportControl.cpp"
)

add_executable(TransportTest ${SOURCES})
target_link_libraries(TransportTest
   Transport
   gtest 
   pthread
)