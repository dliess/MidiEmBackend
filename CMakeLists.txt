cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

project(MidiEmBackend LANGUAGES CXX)
include(GNUInstallDirs)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 7.0.3
  GIT_TAG 76e97dc4df0517749da366a2f78ebb4f87977804
)

CPMAddPackage(
  NAME docopt
  GITHUB_REPOSITORY docopt/docopt.cpp
  VERSION 0.6.3
)

CPMAddPackage(
  NAME rtmidi
  GITHUB_REPOSITORY dliess/rtmidi
  GIT_TAG 27f55e39deae3cdf7ddb43d304b1e432199e4517
  OPTIONS "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
  NAME variant
  GITHUB_REPOSITORY mpark/variant
  VERSION 1.4.0
)

CPMAddPackage(
  NAME loguru
  GITHUB_REPOSITORY emilk/loguru
  VERSION 2.1.0
)

if (loguru_ADDED)
  add_library(Loguru STATIC "${loguru_SOURCE_DIR}/loguru.cpp")
  target_include_directories(Loguru PUBLIC ${loguru_SOURCE_DIR})
  target_link_libraries(Loguru PRIVATE pthread dl)
  target_compile_definitions(Loguru PUBLIC "LOGURU_USE_FMTLIB=1")
  target_link_libraries(Loguru PUBLIC fmt)
endif()

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG b155875f32dc74e293d96c0de2dfcdfa913804e4
)

CPMAddPackage(
  NAME metastuff
  GITHUB_REPOSITORY eliasdaler/MetaStuff
  GIT_TAG c883f789596a3231bf357bedbda81a389cf73d0c
)

if(metastuff_ADDED)
  add_library(Serialize INTERFACE)
  target_include_directories(Serialize
      INTERFACE
          ${metastuff_SOURCE_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/3rdparty/nlohmann_json
  )
endif()

CPMAddPackage(
  NAME Midi
  GITHUB_REPOSITORY dliess/Midi
  GIT_TAG d9501077f9d86dcf1a20e6655c78e45308b74150
)

CPMAddPackage(
  NAME timemeasure
  GITHUB_REPOSITORY dliess/TimeMeasure
  GIT_TAG develop
)

CPMFindPackage(
  NAME midiem-capnzero
  #GITHUB_REPOSITORY dliess/MidiEmCapnZero
  GIT_REPOSITORY git@github.com:dliess/MidiEmCapnZero.git
  GIT_TAG 8f9f973953a9b6eab4257cddd6d176db7515bfbf
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
  gtest
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MidiEmBackendBackend
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY "MidiConfigs"
  DESTINATION "/etc/midiem-backend/"
)