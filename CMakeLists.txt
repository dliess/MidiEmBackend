cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)
include(GNUInstallDirs)

project(MidiEmBackend)

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
  NAME loguru
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
  GIT_TAG 9a78902e6db849b21c50d735b9bd132e4d8ff56c
)

CPMAddPackage(
  NAME timemeasure
  GITHUB_REPOSITORY dliess/TimeMeasure
  GIT_TAG ca69776f75fe7775943c411a4e3cf624d37c67f8
)

CPMFindPackage(
  NAME cppzmq
  GITHUB_REPOSITORY zeromq/cppzmq
  VERSION 4.7.0
  OPTIONS
   "CPPZMQ_BUILD_TESTS OFF"
)

CPMFindPackage(
  NAME grpc
  GITHUB_REPOSITORY grpc/grpc
  VERSION 1.28.0
  OPTIONS
    "BUILD_TESTING OFF"
    "gRPC_ZLIB_PROVIDER module"
    "gRPC_CARES_PROVIDER module"
    "gRPC_RE2_PROVIDER module"
    "gRPC_SSL_PROVIDER module"
    "gRPC_PROTOBUF_PROVIDER module"
    "gRPC_ABSL_PROVIDER module"
    "gRPC_BUILD_GRPC_CPP_PLUGIN ON"
    "gRPC_BUILD_TESTS OFF"
    "gRPC_BUILD_CSHARP_EXT OFF"
    "gRPC_BUILD_GRPC_CPP_PLUGIN ON"
    "gRPC_BUILD_GRPC_CSHARP_PLUGIN OFF"
    "gRPC_BUILD_GRPC_NODE_PLUGIN OFF"
    "gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN OFF"
    "gRPC_BUILD_GRPC_PHP_PLUGIN OFF"
    "gRPC_BUILD_GRPC_PYTHON_PLUGIN ON"
    "gRPC_BUILD_GRPC_RUBY_PLUGIN OFF"
)

if (grpc_ADDED)
   if (NOT TARGET gRPC::grpc_cpp_plugin)
      add_executable(gRPC::grpc_cpp_plugin ALIAS grpc_cpp_plugin)
   endif()
   if (NOT TARGET gRPC::grpc++_reflection)
      add_library(gRPC::grpc++_reflection ALIAS grpc++_reflection)
   endif()
endif()

add_subdirectory(3rdparty)
add_subdirectory(Base)
add_subdirectory(Util)
add_subdirectory(MusicDevicePlugins)
add_subdirectory(UIAdapter)

set(SOURCES
   "main.cpp"
)

set(TARGET_NAME ${PROJECT_NAME}Backend)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME}
  #UIAdaptGrpcServer
  UIAdapterQt
  Base
  docopt_s
  gtest
)

install(TARGETS ${TARGET_NAME}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
