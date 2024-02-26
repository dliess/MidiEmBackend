# if(NOMIDI_BACKEND_BUILD_TESTS)
#    add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
# endif()

set(SOURCES
#   "KitInstrumentsModifier.cpp"
#   "KitInstrumentModifier.cpp"
#   "KitInstrument.cpp"
   "KitInstrumentComponent.cpp"
   #   "MelodicInstrumentsModifier.cpp"
   #   "MelodicInstrumentModifier.cpp"
   #   "MelodicInstrument.cpp"
   #   "InstrumentsPersister.cpp"
)

set(HEADERS
   # "Instrument.h"
   # "InstrumentsData.h"
   # "InstrumentsDataMeta.h"
   # "KitInstrumentsModifier.h"
   # "KitInstrumentModifier.h"
   # "InstrumentsPersister.h"
   # "KitInstrument.h"
   # "KitInstrument.inl"
   # "KitInstrumentMeta.h"
   # "KitInstrumentVoice.h"
   # "KitInstrumentVoice.inl"
   # "KitInstrumentVoiceMeta.h"
    "KitInstrumentComponent.h"
    "KitInstrumentComponent.inl"
   # "KitInstrumentComponentMeta.h"
   # "MelodicInstrument.h"
   # "MelodicInstrument.inl"
   # "MelodicInstrumentMeta.h"
   # "MelodicInstrumentsModifier.h"
   # "MelodicInstrumentModifier.h"
)

set(TARGET_NAME "InstrumentsRt")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PUBLIC  MusicDevice
   PRIVATE MIDI::usbmidi
   PRIVATE spdlog::spdlog
   PUBLIC  util
   PUBLIC  magic_enum::magic_enum
   PUBLIC  farbot::farbot
   PUBLIC  tl::expected
)
