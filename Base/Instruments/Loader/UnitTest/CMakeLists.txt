#add_subdirectory(Mock)

set(SOURCES
   "UT_MelodicInstrument.cpp"
   "UT_MelodicSerialization.cpp"
   "UT_KitSerialization.cpp"
)

set(TARGET_NAME InstrumentsLoaderTest)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   InstrumentsLoader
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
