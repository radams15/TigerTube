cmake_minimum_required(VERSION 2.8)
project(youtubecli CXX)

set(CMAKE_CXX_STANDARD 11) # 98

set(CURL_LIBRARIES "-lcurl -L/usr/local/opt/curl/lib")
set(CURL_INCLUDE_DIRS "-I/usr/local/opt/curl/include")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
#pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(XML REQUIRED libxml-2.0)

include_directories(regex regex/includes ${CURL_INCLUDE_DIRS} ${XML_INCLUDE_DIRS})

add_executable(youtubecli main.cpp  Net.cpp Net.h Channel.cpp Channel.h Video.cpp Video.h Config.cpp Config.h Fs.cpp Fs.h regex/Regex.cpp regex/RegexUtils.cpp regex/RegexUtils.hpp MainWindow.cpp MainWindow.h VideoTable.cpp VideoTable.h)

target_link_libraries(youtubecli PRIVATE Qt4::QtGui ${CURL_LIBRARIES} ${XML_LIBRARIES})
