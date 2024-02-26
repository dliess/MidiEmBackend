#add_subdirectory(Mock)

set(SOURCES
)

set(TARGET_NAME InstrumentsRtTest)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   InstrumentsRt
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
