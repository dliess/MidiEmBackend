cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

project(MidiEmBackend LANGUAGES CXX)
include(GNUInstallDirs)

include(CTest)
option (MIDIEM_BACKEND_BUILD_TESTS "build tests enabled" ON)
if (${MIDIEM_BACKEND_BUILD_TESTS})
   message(STATUS "Build with Unit Tests")
   enable_testing()
endif()

include(cmake/CPM.cmake)

# WORKAROUND: get Midi before "fmt"
CPMFindPackage(
  NAME midi
  #GITHUB_REPOSITORY dliess/Midi
  GIT_REPOSITORY git@danita-hp:app/Midi.git
  GIT_TAG master
)
if(midi_ADDED)
  add_library(MIDI::midi ALIAS midi)
  add_library(MIDI::usbmidi ALIAS usbmidi)
endif()

CPMFindPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 6.2.0
  GIT_TAG 9bdd1596cef1b57b9556f8bef32dc4a32322ef3e
)

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

find_package(spdlog CONFIG REQUIRED)

if(${MIDIEM_BACKEND_BUILD_TESTS})
CPMFindPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG b155875f32dc74e293d96c0de2dfcdfa913804e4
)
endif()

CPMFindPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.7.0
)
if(nlohmann_json_ADDED)
  add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
endif()

CPMFindPackage(
  NAME MetaStuff
  GITHUB_REPOSITORY eliasdaler/MetaStuff
  #GIT_TAG 70652dc73fe6268ef304cb062884a330a27c9f6f
  GIT_TAG c883f789596a3231bf357bedbda81a389cf73d0c
  OPTIONS
    "WITH_EXAMPLES OFF"
)
if(MetaStuff_ADDED)
  add_library(MetaStuff INTERFACE)
  target_include_directories(MetaStuff
      INTERFACE
          ${MetaStuff_SOURCE_DIR}/include
)
endif()

CPMFindPackage(
  NAME timemeasure
  GITHUB_REPOSITORY dliess/TimeMeasure
  GIT_TAG develop
)

CPMFindPackage(
  NAME MidiEmCapnZero
  #GITHUB_REPOSITORY dliess/MidiEmCapnZero
  #GIT_REPOSITORY git@github.com:dliess/MidiEmCapnZero.git
  GIT_REPOSITORY git@danita-hp:app/MidiEmCapnZero.git
  GIT_TAG develop
)
if(MidiEmCapnZero_ADDED)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-loader ALIAS midiem-capnzero-loader)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-rt ALIAS midiem-capnzero-rt)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-loader-qt ALIAS midiem-capnzero-loader-qt)
  add_library(MIDIEMCAPNZERO::midiem-capnzero-rt-qt ALIAS midiem-capnzero-rt-qt)
endif()

CPMFindPackage(
  NAME ableton-link
  GITHUB_REPOSITORY Ableton/link
  GIT_TAG master
  DOWNLOAD_ONLY YES
)
if(ableton-link_ADDED)
  include(${ableton-link_SOURCE_DIR}/AbletonLinkConfig.cmake)
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

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MidiEmBackendBackend
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY "MidiConfigs"
  DESTINATION "/etc/midiem-backend/"
)