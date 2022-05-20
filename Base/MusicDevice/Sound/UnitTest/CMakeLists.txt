set(SOURCES
   "UT_main.cpp"
   "UT_DevicePresets.cpp"
   "UT_SoundHandler.cpp"
   "UT_SoundMidiInMsgHandler.cpp"
   "UT_SoundMidiOutMsgHandler.cpp"
   "UT_SoundParameterStorage.cpp"
   "UT_SoundPresetHandler.cpp"
)

set(TARGET_NAME "MusicDeviceSoundTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDeviceSound
   MusicDeviceDescription
   gtest 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})