set(SOURCES 
    "UT_itc.cpp"
    "UT_2DMatrixCheck.cpp"
    "UT_FixedSizeVector.cpp"
    "UT_KeyValueVector.cpp"
    "UT_JsonParseOptionals.cpp"
    "UT_JsonParseVariants.cpp"
    "UT_Overload.cpp"
    "UT_Ringbuffer.cpp"
    "UT_ReflectionWithNamespace.cpp"
    "UT_ThreadHelpers.cpp"
)

add_executable(UtilsTest ${SOURCES})
target_link_libraries(UtilsTest
    GTest::GTest
    GTest::Main 
    pthread 
    Util 
    MIDI::midi
)

