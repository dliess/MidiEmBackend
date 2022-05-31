set(SOURCES
   "UT_PluginHandler.cpp"
)

set(TARGET_NAME "MusicDevicePluginHandlerTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDevicePluginHandler
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})