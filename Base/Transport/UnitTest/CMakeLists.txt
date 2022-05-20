set(SOURCES
   "UT_main.cpp"
   "UT_TransportControl.cpp"
)

set(TARGET_NAME "TransportTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   Transport
   gtest 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})