if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "Instruments.cpp"
   "InstrumentsFactory.cpp"
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
)

set(HEADERS
   "CompositeSound.h"
   "CompositeSound.inl"
   "VoiceDescr.h"
   "VoiceDescr.inl"
)

add_library(Instruments STATIC ${SOURCES} ${HEADERS})

target_include_directories(Instruments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Instruments
   PUBLIC  MusicDevice
   PRIVATE MIDI::usbmidi
   PRIVATE spdlog::spdlog
   PUBLIC  util
)