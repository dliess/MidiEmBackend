set(SOURCES
   "UT_main.cpp"
   "UT_MusicDevice.cpp"
)

set(TARGET_NAME "MusicDeviceTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDevice
   MusicDeviceDescription
   gtest 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})