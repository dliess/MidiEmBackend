set(SOURCES
   "UT_musicDeviceDescription.cpp"
)

set(TARGET_NAME "MusicDeviceDescriptionTest")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   MusicDeviceDescription
   GTest::GTest
   GTest::Main 
   pthread
   fmt
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})

target_compile_definitions(${TARGET_NAME} PRIVATE -DRESOURCE_ROOT_DIR="${PROJECT_SOURCE_DIR}")