file(RELATIVE_PATH DEV_DIR "${PROJECT_SOURCE_DIR}/MusicDevicePlugins/Devices/" "${CMAKE_CURRENT_SOURCE_DIR}")
string(REPLACE "/" "__" TARGET_NAME "${DEV_DIR}")

set(SOURCES
   "plugin.cpp"
)

add_library(${TARGET_NAME} SHARED ${SOURCES})
target_link_libraries(${TARGET_NAME}
   PRIVATE MusicDevicePluginInterface
   PRIVATE util
   PRIVATE MusicDeviceController
   PRIVATE MusicDeviceDescription
)
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "plugin")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> "${CMAKE_CURRENT_SOURCE_DIR}"
)

#install(
#   TARGETS ${TARGET_NAME}
#      LIBRARY
#         DESTINATION "${PROJECT_SOURCE_DIR}/MusicDevicePlugins/Devices/"
#)