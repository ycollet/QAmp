#=============================================================================
#  QAmp
#  Qt LV2 Amplifier
#  $Id:$
#
#  Copyright (C) 2016 by Yann Collette and others
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#=============================================================================

project(QAmp C CXX)

cmake_minimum_required(VERSION 3.0.3)

option(ENABLE_QT5 "Activate Qt5 build" ON)
option(COMPILE_TEST "Compile the test executable" ON)

set(CMAKE_AUTOMOC OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(QAMP_SRCS    amp.c)
set(QAMP_UI_SRCS QAmp.cpp qamp_gui.cpp)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LV2 REQUIRED lv2>=1.10)

include_directories("${CMAKE_SOURCE_DIR}/src"
		    "${LV2_INCLUDEDIR}")

if (ENABLE_QT5)
  set(QTGUILV2 "Qt5")
  
  find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

  message(STATUS "DEBUG: Qt5Widgets_INCLUDE_DIRS = ${Qt5Widgets_INCLUDE_DIRS}")
  message(STATUS "DEBUG: Qt5Core_INCLUDE_DIRS = ${Qt5Core_INCLUDE_DIRS}")
  message(STATUS "DEBUG: Qt5Widgets_DEFINITIONS = ${Qt5Widgets_DEFINITIONS}")
  message(STATUS "DEBUG: Qt5Core_DEFINITIONS = ${Qt5Core_DEFINITIONS}")
  message(STATUS "DEBUG: Qt5Widgets_LIBRARIES = ${Qt5Widgets_LIBRARIES}")
  message(STATUS "DEBUG: Qt5Core_LIBRARIES = ${Qt5Core_LIBRARIES}")
  
  include_directories(${CMAKE_SOURCE_DIR}
                      ${Qt5Widgets_INCLUDE_DIRS}
                      ${Qt5Core_INCLUDE_DIRS})

  add_definitions(-D_REENTRANT -DQT_NO_DEBUG -DQT5 ${Qt5Widgets_DEFINITIONS} ${Qt5Core_DEFINITIONS})
  
  qt5_generate_moc(QAmp.h ${CMAKE_BINARY_DIR}/moc_QAmp_qt5.cpp)
  qt5_generate_moc(test/MainWindow.h ${CMAKE_BINARY_DIR}/moc_MainWindow_qt5.cpp)
  
  add_library(qamp-gui SHARED ${QAMP_UI_SRCS}
  			      ${CMAKE_BINARY_DIR}/moc_QAmp_qt5.cpp)
  
  set_target_properties(qamp-gui PROPERTIES PREFIX "")
  
  target_link_libraries(qamp-gui ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${LV2_LDFLAGS})

  if (COMPILE_TEST)
    add_executable(main-test test/main-test.cpp
                             test/MainWindow.cpp
			     ${CMAKE_BINARY_DIR}/moc_MainWindow_qt5.cpp
			     ${CMAKE_BINARY_DIR}/moc_QAmp_qt5.cpp
			     QAmp.cpp)
  endif ()
  
  target_link_libraries(main-test ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES})
  
  install(TARGETS qamp-gui
          DESTINATION lib64/lv2/qamp.lv2)
else ()
  set(QTGUILV2 "Qt4")
  
  find_package(Qt4 4.8.0 REQUIRED QtGui QtCore)

  add_definitions(-D_REENTRANT -DQT_NO_DEBUG ${QT_DEFINITIONS})
  
  include_directories(${CMAKE_SOURCE_DIR}
                      ${QT_INCLUDES})
  
  qt4_generate_moc(QAmp.h ${CMAKE_BINARY_DIR}/moc_QAmp_qt4.cpp)
  qt4_generate_moc(MainWindow.h ${CMAKE_BINARY_DIR}/moc_MainWindow_qt4.cpp)
  
  add_library(qamp-gui SHARED ${QAMP_UI_SRCS}
			      ${CMAKE_BINARY_DIR}/moc_QAmp_qt4.cpp)
  
  set_target_properties(qamp-gui PROPERTIES PREFIX "")
  
  target_link_libraries(qamp-gui ${QT_QTGUI_LIBRARY} ${QT_CORE_LIBRARY} ${LV2_LDFLAGS})

  if (COMPILE_TEST)
    add_executable(main-test test/main-test.cpp
                             test/MainWindow.cpp
  			     ${CMAKE_BINARY_DIR}/moc_MainWindow_qt4.cpp
			     ${CMAKE_BINARY_DIR}/moc_QAmp_qt4.cpp
			     QAmp.cpp)
  endif()
  
  target_link_libraries(main-test ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})
  
  install(TARGETS qamp-gui
          DESTINATION lib64/lv2/qamp.lv2)
endif()

add_library(qamp SHARED ${QAMP_SRCS})
set_target_properties(qamp PROPERTIES PREFIX "")
target_link_libraries(qamp ${LV2_LDFLAGS})

configure_file(${CMAKE_SOURCE_DIR}/qamp.ttl ${CMAKE_BINARY_DIR}/qamp.ttl)

install(TARGETS qamp
        DESTINATION lib64/lv2/qamp.lv2)

install(FILES ${CMAKE_BINARY_DIR}/qamp.ttl manifest.ttl
        DESTINATION lib64/lv2/qamp.lv2)


