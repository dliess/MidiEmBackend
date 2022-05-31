set(SOURCES
   "UT_MidiDumper.cpp"
   "UT_MidiRouter.cpp"
)

set(TARGET_NAME "MidiFriendsTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MidiFriends
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})