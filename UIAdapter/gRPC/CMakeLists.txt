cmake_minimum_required(VERSION 3.15)

option(USE_SYSTEM_GRPC "Use system installed gRPC" ON)
if(USE_SYSTEM_GRPC)
   # Find system-installed gRPC
   find_package(Protobuf CONFIG REQUIRED)
   message(STATUS "Using protobuf ${protobuf_VERSION}")
   find_package(gRPC CONFIG REQUIRED)
   message(STATUS "Using gRPC ${gRPC_VERSION}")
else()
  # BUILD WITH: cmake -DBUILD_TESTING=OFF
  # Build gRPC using FetchContent or add_subdirectory
  include(FetchContent)

  FetchContent_Declare(
    gRPC
    GIT_REPOSITORY https://github.com/grpc/grpc
    GIT_TAG        v1.28.0
    )
   set(FETCHCONTENT_QUIET OFF)
   FetchContent_MakeAvailable(gRPC)  
endif()

find_package(Threads)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(protobuf_generate_grpc_cpp)
protobuf_generate_grpc_cpp(GEN_DIRS GEN_SOURCES GEN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/protos/BaseService.proto")

message(INFO " adding to includes: ${GEN_DIRS}")
add_library(UIAdaptGrpcServer STATIC
   "gRPCServer.cpp"
   "gRPCServerAsync.cpp"
   ${GEN_SOURCES}
   ${GEN_HEADERS}
)

target_include_directories(UIAdaptGrpcServer
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
   PUBLIC ${GEN_DIRS}
)

target_link_libraries(UIAdaptGrpcServer 
   PUBLIC grpc++
   PRIVATE Loguru
)