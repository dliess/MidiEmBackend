set(SOURCES
   "NoteMapper.cpp"
   "ParameterMapper.cpp"
   "ValueCurve.cpp"
)

add_library(Mapping STATIC ${SOURCES})

target_include_directories(Mapping PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Mapping
   PUBLIC  MusicDevice
   PUBLIC  Instruments
   PRIVATE spdlog::spdlog
   PUBLIC  util
)