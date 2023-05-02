add_subdirectory(Mock)

set(SOURCES
   "UT_Instruments.cpp"
   "UT_InstrumentSerialization.cpp"
   "UT_InstrumentsMDChangeHandler.cpp"
   "UT_KitInstrument.cpp"
   "UT_MelodicInstrument.cpp"
)

set(TARGET_NAME InstrumentsTest)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   Instruments
   GTest::GTest
   GTest::Main 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})