find_package(gRPC CONFIG REQUIRED)

function(protobuf_generate_grpc_cpp GENDIR SOURCES HEADERS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_GRPC_CPP() called without any proto files")
    return()
  endif()

  # Create directory for generated .proto files
  set(_gRPC_PROTO_GENS_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
  file(MAKE_DIRECTORY ${_gRPC_PROTO_GENS_DIR})

  set(${GENDIR})
  set(${SOURCES})
  set(${HEADERS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)
    file(RELATIVE_PATH REL_FIL ${CMAKE_CURRENT_SOURCE_DIR} ${ABS_FIL})
    get_filename_component(REL_DIR ${REL_FIL} DIRECTORY)
    set(RELFIL_WE "${REL_DIR}/${FIL_WE}")

    if(CMAKE_CROSSCOMPILING)
      find_program(_gRPC_CPP_PLUGIN grpc_cpp_plugin)
      find_program(_gRPC_PROTOBUF_PROTOC_EXECUTABLE protoc)
    else()
      set(_gRPC_CPP_PLUGIN $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
      set(_gRPC_PROTOBUF_PROTOC_EXECUTABLE "$<TARGET_FILE:protobuf::protoc>")
    endif()

    add_custom_command(
       OUTPUT "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.cc"
              "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.h"
              "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.cc"
              "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.h"
       COMMAND ${_gRPC_PROTOBUF_PROTOC_EXECUTABLE}
       ARGS --grpc_out=${_gRPC_PROTO_GENS_DIR}
            --cpp_out=${_gRPC_PROTO_GENS_DIR}
            --plugin=protoc-gen-grpc=${_gRPC_CPP_PLUGIN}
            -I ${CMAKE_CURRENT_SOURCE_DIR}
            ${REL_FIL}
       DEPENDS ${ABS_FIL}
       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
       COMMENT "Running gRPC C++ protocol buffer compiler on ${FIL}"
       VERBATIM
    )
    list(APPEND ${SOURCES} "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.cc")
    list(APPEND ${SOURCES} "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.cc")
    list(APPEND ${HEADERS} "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.h")
    list(APPEND ${HEADERS} "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.h")
  endforeach()

  set(${GENDIR} ${_gRPC_PROTO_GENS_DIR} PARENT_SCOPE)
  set(${SOURCES} ${${SOURCES}} PARENT_SCOPE)
  set(${HEADERS} ${${HEADERS}} PARENT_SCOPE)
endfunction()
