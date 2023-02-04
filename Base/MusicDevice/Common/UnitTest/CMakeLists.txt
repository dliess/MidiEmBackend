set(SOURCES
   "UT_MusicDeviceIdSerialize.cpp"
)

set(TARGET_NAME "MusicDeviceCommonTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDeviceCommon
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})