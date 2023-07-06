if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()

set(SOURCES
   "MusicDeviceDescription.cpp"
)

set(HEADERS
   "ControllerSection.h"
   "ControllerSection.inl"
   "ControllerSectionMeta.h"
   "ControllerSysExDescription.h"
   "ControllerSysExDescriptionMeta.h"
   "HubSection.h"
   "HubSectionMeta.h"
   "MidiMessageIdsMeta.h"
   "MidiSysexDumpDescrHelper.h"
   "MusicDeviceChains.h"
   "MusicDeviceChainsMeta.h"
   "MusicDeviceDescription.h"
   "MusicDeviceDescriptionMeta.h"
   "SoundSection.h"
   "SoundSection.inl"
   "SoundSectionMeta.h"
   "SoundSysExDescription.h"
   "SoundSysExDescriptionMeta.h"
   "SysExDescription.h"
   "SysExDescriptionMeta.h"
)

add_library(MusicDeviceDescription STATIC ${SOURCES} ${HEADERS})
target_include_directories(MusicDeviceDescription PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MusicDeviceDescription
   PUBLIC Util
   PUBLIC MIDI::midi
   PRIVATE MusicDeviceCommon
)