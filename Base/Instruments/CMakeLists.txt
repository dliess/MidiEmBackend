if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "Instruments.cpp"
   "InstrumentsMDChangeHandler.cpp"
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
   "InstrumentsModifier.cpp"
   "InstrumentVoice.cpp"
   "InstrumentVoiceFactory.cpp"
   "InstrumentsMDRefSetter.cpp"
   "InstrumentsPersister.cpp"
)

set(HEADERS
   "CompositeSound.h"
   "CompositeSound.inl"
   "CompositeSoundMeta.h"
   "InstrumentVoice.h"
   "InstrumentVoice.inl"
   "InstrumentVoiceMeta.h"
   "InstrumentVoiceFactory.h"
   "INotePlayer.h"
   "Instruments.h"
   "Instruments.inl"
   "InstrumentsRef.h"
   "InstrumentsModifier.h"
   "InstrumentsMDRefSetter.h"
   "InstrumentsPersister.h"
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