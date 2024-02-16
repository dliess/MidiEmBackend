if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
#   "InstrumentsMDRefSetter.cpp"
   "KitInstrumentsModifier.cpp"
   "KitInstrumentModifier.cpp"
   "MelodicInstrumentsModifier.cpp"
   "MelodicInstrumentModifier.cpp"
#   "InstrumentsPersister.cpp"
   "KitInstrument.cpp"
   "KitInstrumentComponent.cpp"
   "MelodicInstrument.cpp"
)

set(HEADERS
#   "Instrument.h"
#   "InstrumentsData.h"
#   "InstrumentsDataMeta.h"
   "KitInstrumentsModifier.h"
   "KitInstrumentModifier.h"
#   "InstrumentsMDRefSetter.h"
#   "InstrumentsPersister.h"
#   "InstrumentComponentParameterCacheCreator.h"
#   "InstrumentComponentParameterCacheCreator.inl"
   "KitInstrument.h"
   "KitInstrument.inl"
   "KitInstrumentMeta.h"
#   "KitInstrumentVoice.h"
#   "KitInstrumentVoice.inl"
#   "KitInstrumentVoiceMeta.h"
#   "KitInstrumentComponent.h"
#   "KitInstrumentComponent.inl"
#   "KitInstrumentComponentMeta.h"
   "MelodicInstrument.h"
   "MelodicInstrument.inl"
   "MelodicInstrumentMeta.h"
   "MelodicInstrumentsModifier.h"
   "MelodicInstrumentModifier.h"
)

set(TARGET_NAME "InstrumentsLoader")

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
