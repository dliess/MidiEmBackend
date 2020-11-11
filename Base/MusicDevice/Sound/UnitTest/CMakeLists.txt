cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
   "UT_main.cpp"
   "UT_SoundHandler.cpp"
   "UT_SoundMidiInMsgHandler.cpp"
   "UT_SoundMidiOutMsgHandler.cpp"
   "UT_SoundParameterStorage.cpp"
   "UT_SoundPresetHandler.cpp"
)

add_executable(MusicDeviceSoundTest ${SOURCES})
target_link_libraries(MusicDeviceSoundTest
   MusicDeviceSound
   MusicDeviceDescription
   gtest 
   pthread
)