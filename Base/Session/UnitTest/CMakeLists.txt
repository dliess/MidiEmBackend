set(SOURCES
   "UT_Serialize.cpp"
)

set(TARGET_NAME "SessionTests")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   Session
   InstrumentsMock
   Instruments
   GTest::GTest
   ${GMOCK_LIBRARIES}
   GTest::Main
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})