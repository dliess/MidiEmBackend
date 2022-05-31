set(SOURCES
   "UT_BeatTick.cpp"
   "UT_TapTempoHandler.cpp"
)

set(TARGET_NAME "TempoTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   Tempo
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})