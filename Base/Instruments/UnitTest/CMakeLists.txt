set(SOURCES
   "UT_main.cpp"
   "UT_Instruments.cpp"
   "UT_InstrumentsFactory.cpp"
   "UT_KitInstrument.cpp"
   "UT_MelodicInstrument.cpp"
)

set(TARGET_NAME InstrumentsTest)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
   Instruments
   gtest 
   pthread
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})