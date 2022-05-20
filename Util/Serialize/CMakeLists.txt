set(SOURCES
    "StringCast.cpp"
)

add_library(UtilSerialize STATIC ${SOURCES})
target_include_directories(UtilSerialize
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UtilSerialize
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC MetaStuff::MetaStuff
    PUBLIC fmt
)