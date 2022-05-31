cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

project(NomidiBackend LANGUAGES CXX)
include(GNUInstallDirs)

include(CTest)
option (NOMIDI_BACKEND_BUILD_TESTS "build tests enabled" ON)
if (${NOMIDI_BACKEND_BUILD_TESTS})
   message(STATUS "Build with Unit Tests")
   enable_testing()
endif()

include(cmake/CPM.cmake)

# WORKAROUND: get Midi before "fmt"
CPMFindPackage(
  NAME midi
  GITHUB_REPOSITORY dliess/Midi
  #GIT_REPOSITORY git@danita-hp:app/Midi.git
  GIT_TAG 1032a3fa13d3fe02fbe71f202f032e7074503f7b
)
if(midi_ADDED)
  add_library(MIDI::midi ALIAS midi)
  add_library(MIDI::usbmidi ALIAS usbmidi)
endif()

CPMFindPackage(
  NAME docopt
  GITHUB_REPOSITORY docopt/docopt.cpp
  VERSION 0.6.3
)

CPMFindPackage(
  NAME mpark_variant
  GITHUB_REPOSITORY mpark/variant
  VERSION 1.4.0
)

find_package(spdlog 1.8.1 CONFIG REQUIRED)

if(${NOMIDI_BACKEND_BUILD_TESTS})
  find_package(GTest REQUIRED)
endif()

CPMFindPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  #VERSION 3.10.5 bleeding edge in repo
  #VERSION 3.10.2
  # hardknott:
  VERSION 3.9.1 
  #VERSION 3.7.0 dunfell
)
if(nlohmann_json_ADDED)
  add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
endif()

CPMFindPackage(
  NAME MetaStuff
  GIT_REPOSITORY git@github.com:dliess/MetaStuff.git
  GIT_TAG develop
  OPTIONS
    "WITH_EXAMPLES OFF"
)
if(MetaStuff_ADDED)
  add_library(MetaStuff::MetaStuff ALIAS MetaStuff)
endif()

CPMFindPackage(
  NAME TimeMeasure
  GITHUB_REPOSITORY dliess/TimeMeasure
  GIT_TAG develop
)
if(TimeMeasure_ADDED)
  add_library(TimeMeasure::TimeMeasure ALIAS TimeMeasure)
endif()

CPMFindPackage(
  NAME MidiEmCapnZero
  GIT_REPOSITORY git@github.com:dliess/MidiEmCapnZero.git
  GIT_TAG b655ad42f96f644f1257087044516758ed554fec
)
if(MidiEmCapnZero_ADDED)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-loader ALIAS midiem-capnzero-loader)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-rt ALIAS midiem-capnzero-rt)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-loader-qt-wc ALIAS midiem-capnzero-loader-qt-wc)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-rt-qt-wc ALIAS midiem-capnzero-rt-qt-wc)
endif()

CPMFindPackage(
  NAME AbletonLink
  GIT_REPOSITORY git@github.com:dliess/link.git
  GIT_TAG dliess/develop
)
if(AbletonLink_ADDED)
  add_library(AbletonLink::AbletonLink ALIAS AbletonLink)
endif()

add_subdirectory(3rdparty)
add_subdirectory(Base)
add_subdirectory(Util)
add_subdirectory(MusicDevicePlugins)
add_subdirectory(UIAdapter)

set(SOURCES
   "main.cpp"
)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME}
  UIAdapterQt
  Base
  docopt_s
)

install(TARGETS ${TARGET_NAME}  
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY "MidiConfigs"
  DESTINATION "/etc/nomidi-backend/"
)

set(SYSTEM_UNIT_FILES
   "systemd/nomidi-backend.service"
)

install (FILES ${SYSTEM_UNIT_FILES} 
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system
)