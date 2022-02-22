cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 17)

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
   PUBLIC midiem-capnzero-rt-qt
   PUBLIC midiem-capnzero-loader-qt
)