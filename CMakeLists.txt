cmake_minimum_required(VERSION 2.8.11)
project(evo-2048)

set(C_FLAGS "-g -Wall -fPIC" CACHE STRING "")
set(CXX_FLAGS "${C_FLAGS} -std=c++11 -fno-exceptions" CACHE STRING "")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${C_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CXX_FLAGS})

if(${WIN32})
	set(QT5_INCLUDE "C:/Qt/5.7/mingw53_32/include" CACHE STRING "")
	set(QT5_LINK "C:/Qt/5.7/mingw53_32/lib" CACHE STRING "")
else()
	set(QT5_INCLUDE "/usr/include/x86_64-linux-gnu/qt5" CACHE STRING "")
	set(QT5_LINK "" CACHE STRING "")
endif()

set(LIBLA "libla" CACHE INTERNAL "")
include_directories(${LIBLA})

include_directories(${QT5_INCLUDE} ${QT5_INCLUDE}/QtWidgets ${QT5_INCLUDE}/QtCore ${QT5_INCLUDE}/QtGui)
link_directories(${QT5_LINK})

add_subdirectory(libgenn-qt)
include_directories(${LIBGENN_QT})
set(LIBS ${LIBS} genn-qt)

include_directories(source)
set(SOURCE source/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

set(LIBS ${LIBS} pthread)
set(LIBS ${LIBS} Qt5Core Qt5Gui Qt5Widgets)

target_link_libraries(${PROJECT_NAME} ${LIBS})
