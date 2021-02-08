function(capnzero_generate_cpp SOURCES HEADERS PROTOCOL_DESCRIPTION_FILE)
  set(_gRPC_PROTO_GENS_DIR ${CMAKE_CURRENT_BINARY_DIR})

  set(${SOURCES})
  set(${HEADERS})

  get_filename_component(FIL_WLE ${PROTOCOL_DESCRIPTION_FILE} NAME_WLE) # File name without directory and last extension

  set(GEN_CAPNP_FILE "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}.capnp")
  file(TOUCH ${GEN_CAPNP_FILE})

  add_custom_command(
    OUTPUT  "${GEN_CAPNP_FILE}"
            "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Client.h"
            "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Client.cpp"
            "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Server.h"
            "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Server.cpp"
#    COMMAND "$<TARGET_FILE:capnzeroc>"
    COMMAND python3 capnzeroc.py
    ARGS  --outdir=${_gRPC_PROTO_GENS_DIR}
          ${PROTOCOL_DESCRIPTION_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running capnzeroc generator script on ${PROTOCOL_DESCRIPTION_FILE}"
    VERBATIM
  )

  find_package(CapnProto CONFIG REQUIRED)
  set(CAPNPC_SRC_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

  if(TARGET capnp_tool)
    set(CAPNP_EXECUTABLE capnp_tool)
    GET_TARGET_PROPERTY(CAPNPC_CXX_EXECUTABLE capnpc_cpp CAPNPC_CXX_EXECUTABLE)
    GET_TARGET_PROPERTY(CAPNP_INCLUDE_DIRECTORY capnp_tool CAPNP_INCLUDE_DIRECTORY)
    list(APPEND tool_depends capnp_tool capnpc_cpp)
  else()
    message(FATAL "no TARGET capnp_tool")
  endif()

  set(include_path -I "${CAPNPC_SRC_PREFIX}" -I "${CAPNP_INCLUDE_DIRECTORY}")
  set(output_base ${CMAKE_CURRENT_BINARY_DIR}/capnp)

  add_custom_command(
    OUTPUT "${output_base}.c++" "${output_base}.h"
    COMMAND capnp_tool
    ARGS compile
        -o ${CAPNPC_CXX_EXECUTABLE}
        --src-prefix ${CAPNPC_SRC_PREFIX}
        ${include_path}
        ${GEN_CAPNP_FILE}
    DEPENDS "${GEN_CAPNP_FILE}" ${tool_depends}
    COMMENT "Compiling Cap'n Proto schema ${schema_file}"
    VERBATIM
  )

  list(APPEND ${SOURCES} "${CAPNP_GEN_SRCS}")
  list(APPEND ${SOURCES} "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Client.cpp")
  list(APPEND ${SOURCES} "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Server.cpp")
  list(APPEND ${HEADERS} "${CAPNP_GEN_HDRS}")
  list(APPEND ${HEADERS} "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Client.h")
  list(APPEND ${HEADERS} "${_gRPC_PROTO_GENS_DIR}/${FIL_WLE}_Server.h")

  set(${SOURCES} ${${SOURCES}} PARENT_SCOPE)
  set(${HEADERS} ${${HEADERS}} PARENT_SCOPE)
endfunction()
