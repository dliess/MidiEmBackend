set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 5.11.3
   COMPONENTS Core WebSockets WebChannel REQUIRED)

set(SOURCES
   "StartQt.cpp"
)

set(HEADERS
   "StartQt.h"
)

add_library(UIAdapterQt STATIC ${SOURCES} ${HEADERS})
target_include_directories(UIAdapterQt PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(UIAdapterQt
   PUBLIC Qt5::Core
   PRIVATE Base
   PRIVATE Util
   PUBLIC MIDI::usbmidi
   PUBLIC Qt5::WebSockets
   PUBLIC Qt5::WebChannel
   PUBLIC QtWebSocketClientWrapper
   PUBLIC MIDIEMCAPNZERO::midiem-capnzero-rt-qt-wc
   PUBLIC MIDIEMCAPNZERO::midiem-capnzero-loader-qt-wc
)

if(NOMIDI_BACKEND_BUILD_TESTS)
   add_subdirectory(UnitTest EXCLUDE_FROM_ALL)
endif()