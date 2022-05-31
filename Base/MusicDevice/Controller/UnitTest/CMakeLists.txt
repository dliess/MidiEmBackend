set(SOURCES
   "UT_ControllerHandler.cpp"
   "UT_ControllerMidiInMsgHandler.cpp"
   "UT_ControllerMidiOutMsgHandler.cpp"
)

set(TARGET_NAME "MusicDeviceControllerTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDeviceController
   MusicDeviceDescription
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})