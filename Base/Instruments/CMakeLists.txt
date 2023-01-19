if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "Instruments.cpp"
   "InstrumentsMDChangeHandler.cpp"
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
   "InstrumentsModifier.cpp"
)

set(HEADERS
   "CompositeSound.h"
   "CompositeSound.inl"
   "InstrumentVoice.h"
   "INotePlayer.h"
   "Instruments.h"
   "InstrumentsRef.h"
   "InstrumentsModifier.h"
)

set(TARGET_NAME "Instruments")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PUBLIC  MusicDevice
   PRIVATE MIDI::usbmidi
   PRIVATE spdlog::spdlog
   PUBLIC  util
)