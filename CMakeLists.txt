cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX _g)

project(NomidiBackend LANGUAGES CXX)
include(GNUInstallDirs)

include(CTest)
option (USE_MPARK_VARIANT "use mpark variant" ON)
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
  GIT_TAG 5e9f3473dd9ab0bb88aed28c7fa61376ad2dc250
  OPTIONS
    "USE_MPARK_VARIANT ${USE_MPARK_VARIANT}"
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
  NAME Variant
  GITHUB_REPOSITORY dliess/variant
  GIT_TAG master
  OPTIONS
    "USE_MPARK_VARIANT ${USE_MPARK_VARIANT}"
)

find_package(spdlog 1.8.1 CONFIG REQUIRED)

if(${NOMIDI_BACKEND_BUILD_TESTS})
  find_package(GTest REQUIRED)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(GMock REQUIRED)
  include_directories(${GMOCK_INCLUDE_DIRS}) 
endif()

CPMFindPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  #VERSION 3.10.5 bleeding edge in repo
  #VERSION 3.10.2
  # hardknott:
  VERSION 3.9.1
  #VERSION 3.7.0 dunfell
  OPTIONS
    "JSON_BuildTests OFF"
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
    "USE_MPARK_VARIANT ${USE_MPARK_VARIANT}"
)

CPMFindPackage(
  NAME MusicDeviceDescriptions 
  GITHUB_REPOSITORY dliess/musicdevicedescriptions
  GIT_TAG 5d6ea7ce117a91062907220ab910dc470bc1ff9a
  OPTIONS
    "USE_MPARK_VARIANT ${USE_MPARK_VARIANT}"
)

# CPMFindPackage(
#   NAME TimeMeasure
#   GITHUB_REPOSITORY dliess/TimeMeasure
#   GIT_TAG develop
# )
# if(TimeMeasure_ADDED)
#   add_library(TimeMeasure::TimeMeasure ALIAS TimeMeasure)
# endif()

CPMFindPackage(
  NAME MidiEmCapnZero
  GIT_REPOSITORY git@github.com:dliess/MidiEmCapnZero.git
  GIT_TAG develop 
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

CPMFindPackage(
  NAME magic_enum
  GITHUB_REPOSITORY Neargye/magic_enum
  VERSION 0.9.0
  OPTIONS
    "MAGIC_ENUM_OPT_INSTALL ON"
)

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

get_target_property(MIDI_CONFIGS_BASEDIR MusicDeviceDescriptions::MusicDeviceDescription CONFIG_FILES_BASEDIR)
message("MIDI_CONFIGS_BASEDIR: " ${MIDI_CONFIGS_BASEDIR})
target_compile_definitions(${TARGET_NAME} PRIVATE
  MIDI_CONFIGS_BASEDIR="${MIDI_CONFIGS_BASEDIR}")

install(TARGETS ${TARGET_NAME}  
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(SYSTEM_UNIT_FILES
   "systemd/nomidi-backend.service"
)

install (FILES ${SYSTEM_UNIT_FILES} 
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system
)
