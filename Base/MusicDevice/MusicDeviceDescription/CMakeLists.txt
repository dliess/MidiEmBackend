cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(SOURCES
   "MusicDeviceDescription.cpp"
   "DeviceDescriptionLoader.cpp"
)

set(HEADERS
   "ControllerSection.h"
   "ControllerSectionMeta.h"
   "DeviceDescriptionLoader.h"
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
)

add_library(MusicDeviceDescription STATIC ${SOURCES} ${HEADERS})
target_include_directories(MusicDeviceDescription PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MusicDeviceDescription
   PUBLIC Util
   PUBLIC midi
   PRIVATE MusicDeviceCommon
)