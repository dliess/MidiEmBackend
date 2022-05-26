find_package(GTest REQUIRED)

set(SOURCES
   "UT_Base64.cpp"
)

set(TARGET_NAME "Base64Test")

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   GTest::GTest
   GTest::Main
   Util
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})