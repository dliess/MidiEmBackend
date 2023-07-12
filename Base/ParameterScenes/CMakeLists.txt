set(TARGET_NAME "ParameterScenes")

set(SOURCES
    "ModifiersApplyer.cpp"
    "ParameterSceneContainer.cpp"
)

set(HEADERS
    "ModifiersApplyer.h"
    "ParameterScene.h"
    "ParameterSceneContainer.h"
    "ParameterSceneContainer.inl" 
)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET_NAME}
    PRIVATE MusicDevice
    PUBLIC Util
    PUBLIC Instruments
)

#if(NOMIDI_BACKEND_BUILD_TESTS)
#   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
#endif()