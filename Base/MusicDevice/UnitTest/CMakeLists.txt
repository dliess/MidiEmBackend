set(SOURCES
   "UT_MusicDevice.cpp"
)

set(TARGET_NAME "MusicDeviceTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDevice
   MusicDeviceDescription
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})