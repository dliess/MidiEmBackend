if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "Instruments.cpp"
   "InstrumentsMDChangeHandler.cpp"
   "KitInstrumentCopyer.cpp"
   "MelodicInstrumentCopyer.cpp"
   )

set(HEADERS
#   "KitInstrumentVoice.h"
#   "KitInstrumentVoice.inl"
#  "KitInstrumentVoiceMeta.h"
#  "MelodicInstrumentVoice.h"
#  "MelodicInstrumentVoice.inl"
#  "MelodicInstrumentVoiceMeta.h"
#   "InstrumentComponent.h"
#   "InstrumentComponent.inl"
#   "InstrumentComponentMeta.h"
#   "InstrumentComponentParameterCacheCreator.h"
#   "InstrumentComponentParameterCacheCreator.inl"
#   "INotePlayer.h"
    "Instruments.h"
    "Instruments.inl"
    "Refs/InstrumentsRef.h"
    "Refs/InstrumentRtRef.h"
    "Refs/KitInstrumentRtRef.h"
    "Refs/MelodicInstrumentRtRef.h"
#   "InstrumentsModifier.h"
#   "InstrumentsMDRefSetter.h"
     "InstrumentsMDChangeHandler.h"
#   "InstrumentsPersister.h"
#   "InstrumentsParameterCache.h"
#   "InstrumentsParameterCache.inl"
   "KitInstrumentCopyer.h"
   "MelodicInstrumentCopyer.h"
)

set(TARGET_NAME "Instruments")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME}
   PUBLIC  InstrumentsLoader
   PUBLIC  InstrumentsRt
   PUBLIC  MusicDevice
   PRIVATE MIDI::usbmidi
   PRIVATE spdlog::spdlog
   PUBLIC  util
   PUBLIC  magic_enum::magic_enum
   PUBLIC  farbot::farbot
)

add_subdirectory(Loader)
add_subdirectory(Rt)
