
#TODO: bring back UT_itc.cpp
set(SOURCES 
    "UT_2DMatrixCheck.cpp"
    "UT_FixedSizeVector.cpp"
    "UT_Function.cpp"
    "UT_FunctionMv.cpp"
    "UT_FunctionRef.cpp"
    "UT_Identifiable.cpp"
    "UT_KeyValueVector.cpp"
    "UT_Json.cpp"
    "UT_JsonMetaNs.cpp"
    "UT_JsonParseOptionals.cpp"
    "UT_JsonParseVariants.cpp"
    "UT_ReflectionWithNamespace.cpp"
    "UT_Ringbuffer.cpp"
    "UT_ThreadHelpers.cpp"
    "UT_Farbot.cpp"
)

add_executable(UtilsTest ${SOURCES})
target_link_libraries(UtilsTest
    GTest::GTest
    GTest::Main 
    pthread 
    Util 
    MIDI::midi
    farbot::farbot
)

