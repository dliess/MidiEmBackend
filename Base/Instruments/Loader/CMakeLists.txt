#if(NOMIDI_BACKEND_BUILD_TESTS)
#   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
#endif()

set(SOURCES
#   "Instruments.cpp"
#   "InstrumentsMDChangeHandler.cpp"
   "KitInstrument.cpp"
   "MelodicInstrument.cpp"
#   "InstrumentsModifier.cpp"
   "InstrumentComponent.cpp"
#   "InstrumentComponentParameterCacheCreator.cpp"
#   "InstrumentsMDRefSetter.cpp"
#   "InstrumentsPersister.cpp"
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
   "InstrumentComponent.h"
   "InstrumentComponent.inl"
   "InstrumentComponentMeta.h"
#   "InstrumentComponentParameterCacheCreator.h"
#   "InstrumentComponentParameterCacheCreator.inl"
#   "INotePlayer.h"
   "Instrument.h"
#   "Instruments.h"
#   "Instruments.inl"
#   "InstrumentsRef.h"
#   "InstrumentsModifier.h"
#   "InstrumentsMDRefSetter.h"
#   "InstrumentsPersister.h"
#   "InstrumentsParameterCache.h"
#   "InstrumentsParameterCache.inl"
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
