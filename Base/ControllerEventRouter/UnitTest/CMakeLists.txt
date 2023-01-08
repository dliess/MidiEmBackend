set(SOURCES
   "UT_ControllerEventRouterLoader.cpp"
   "UT_ControllerEventRouter.cpp"
)

set(TARGET_NAME "ControllerEvtRouterTests")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   GTest::GTest
   GTest::Main
   ${GMOCK_LIBRARIES}
   MusicDeviceControllerEventRouter
   Instruments
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})