#if(NOMIDI_BACKEND_BUILD_TESTS)
#   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
#endif()

set(SOURCES
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
   "InstrumentsModifier.cpp"
   "KitInstrumentComponent.cpp"
   "MelodicInstrumentComponent.cpp"
   "InstrumentsMDRefSetter.cpp"
   "InstrumentsPersister.cpp"
)

set(HEADERS
   "KitInstrument.h"
   "KitInstrument.inl"
   "KitInstrumentMeta.h"
   "KitInstrumentVoice.h"
   "KitInstrumentVoice.inl"
   "KitInstrumentVoiceMeta.h"
   "MelodicInstrument.h"
   "MelodicInstrument.inl"
   "MelodicInstrumentMeta.h"
   "MelodicInstrumentVoice.h"
   "MelodicInstrumentVoice.inl"
   "MelodicInstrumentVoiceMeta.h"
   "KitInstrumentComponent.h"
   "KitInstrumentComponent.inl"
   "KitInstrumentComponentMeta.h"
   "Instrument.h"
   "InstrumentsData.h"
   "InstrumentsDataMeta.h"
   "InstrumentsModifier.h"
   "InstrumentsMDRefSetter.h"
   "InstrumentsPersister.h"
   "MelodicInstrumentComponent.h"
   "MelodicInstrumentComponent.inl"
   "MelodicInstrumentComponentMeta.h"
   "InstrumentComponentParameterCacheCreator.h"
   "InstrumentComponentParameterCacheCreator.inl"
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
)
