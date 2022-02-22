cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

project(MidiEmBackend LANGUAGES CXX)
include(GNUInstallDirs)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 6.2.0
  GIT_TAG 9bdd1596cef1b57b9556f8bef32dc4a32322ef3e
)

CPMAddPackage(
  NAME docopt
  GITHUB_REPOSITORY docopt/docopt.cpp
  VERSION 0.6.2
)

CPMAddPackage(
  NAME mpark_variant
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
  GIT_TAG b97a9e53bede3bb77ea8e63e08668800b9ae8170
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
  GIT_TAG 57e94295b4002ce198d565d398c37248924816bc
)

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
  gtest
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MidiEmBackendBackend
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY "MidiConfigs"
  DESTINATION "/etc/midiem-backend/"
)