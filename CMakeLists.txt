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

set(CMAKE_AUTOMOC OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")

set(QAMP_SRCS amp.c)
set(QAMP_UI_SRCS QAmp.cpp qamp_gui.cpp)
set(QAMP_UI_HEADERS QAmp.h)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LV2 REQUIRED lv2>=1.10)

include_directories("${CMAKE_SOURCE_DIR}/src"
		    "${LV2_INCLUDEDIR}")

if (ENABLE_QT5)
  set(QTGUILV2 "Qt5")
  
  find_package(Qt5 REQUIRED COMPONENTS Widgets)
  
  qt5_generate_moc(QAmp.h QAmp_qt5.cpp)
  
  add_library(qamp-gui SHARED ${QAMP_UI_SRCS}
                              ${QAMP_UI_HEADERS_MOC_QT5}
  			      QAmp_qt5.cpp)
  
  set_target_properties(qamp-gui PROPERTIES PREFIX "")
  set_target_properties(qamp-gui PROPERTIES INCLUDE_DIRECTORIES "${Qt5Widgets_INCLUDE_DIRS}") 
  set_target_properties(qamp-gui PROPERTIES COMPILE_DEFINITIONS "QT5")
  
  target_link_libraries(qamp-gui ${Qt5Widgets_LIBRARIES} ${LV2_LDFLAGS})
  
  install(TARGETS qamp-gui
          DESTINATION lib64/lv2/qamp.lv2)
else ()
  set(QTGUILV2 "Qt4")
  
  find_package(Qt4 4.8.0 REQUIRED QtGui QtCore)
  
  qt4_generate_moc(QAmp.h QAmp_qt4.cpp)
  
  add_library(qamp-gui SHARED ${QAMP_UI_SRCS}
                              ${QAMP_UI_HEADERS_MOC_QT4}
			      QAmp_qt4.cpp)
  
  set_target_properties(qamp-gui PROPERTIES PREFIX "")
  set_target_properties(qamp-gui PROPERTIES INCLUDE_DIRECTORIES "${QT_QTGUI_INCLUDE_DIR};${QT_QTCORE_INCLUDE_DIR}")
  
  target_link_libraries(qamp-gui ${QT_QTGUI_LIBRARY} ${QT_CORE_LIBRARY} ${LV2_LDFLAGS})
  
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


