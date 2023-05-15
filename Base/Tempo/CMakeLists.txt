if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "BeatTick.cpp"
   "TapTempoHandler.cpp"
)

set(TARGET_NAME "Tempo")
add_library(${TARGET_NAME} STATIC ${SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PRIVATE spdlog::spdlog
   PUBLIC  Util
   PUBLIC  AbletonLinkWrapper
)