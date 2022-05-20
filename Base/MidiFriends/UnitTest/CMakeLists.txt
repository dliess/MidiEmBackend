set(SOURCES
   "UT_main.cpp"
   "UT_MidiDumper.cpp"
   "UT_MidiRouter.cpp"
)

set(TARGET_NAME "MidiFriendsTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MidiFriends
   gtest 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})