cmake_minimum_required(VERSION 2.8)
project(youtubecli)

set(CMAKE_CXX_STANDARD 11) # 98

set(CURL_LIBRARY "-lcurl")
find_package(PkgConfig REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base adv)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(XML REQUIRED libxml-2.0)

include_directories(${CURL_INCLUDE_DIRS} ${XML_INCLUDE_DIRS})
include(${wxWidgets_USE_FILE})

add_executable(youtubecli main.cpp  Net.cpp Net.h Channel.cpp Channel.h Video.cpp Video.h Subscriptions.cpp Subscriptions.h Fs.cpp Fs.h MainApp.cpp MainApp.h MainFrame.cpp MainFrame.h Table.cpp Table.h)

target_link_libraries(youtubecli ${CURL_LIBRARIES} ${XML_LIBRARIES} ${wxWidgets_LIBRARIES})
