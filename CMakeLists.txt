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

set(CMAKE_AUTOMOC OFF)

set(QAMP_SRCS amp.c)
set(QAMP_UI_SRCS QAmp.cpp qamp_gui.cpp)
set(QAMP_UI_HEADERS QAmp.h)

find_package(Qt5 COMPONENTS Widgets)
find_package(Qt4 4.8.0 REQUIRED QtGui QtCore)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LV2 REQUIRED lv2>=1.10)

include_directories("${CMAKE_SOURCE_DIR}/src"
		    "${LV2_INCLUDEDIR}")

qt5_generate_moc(QAmp.h QAmp_qt5.cpp)
qt4_generate_moc(QAmp.h QAmp_qt4.cpp)
 
add_library(qamp-qt5 SHARED ${QAMP_UI_SRCS}
                            ${QAMP_UI_HEADERS_MOC_QT5}
			    QAmp_qt5.cpp)

add_library(qamp-qt4 SHARED ${QAMP_UI_SRCS}
                            ${QAMP_UI_HEADERS_MOC_QT4}
			    QAmp_qt4.cpp)

add_library(qamp SHARED ${QAMP_SRCS})

set_target_properties(qamp qamp-qt4 qamp-qt5 PROPERTIES PREFIX "")

set_target_properties(qamp-qt5 PROPERTIES INCLUDE_DIRECTORIES "${Qt5Widgets_INCLUDE_DIRS}") 
set_target_properties(qamp-qt5 PROPERTIES COMPILE_DEFINITIONS "QT5")
set_target_properties(qamp-qt4 PROPERTIES INCLUDE_DIRECTORIES "${QT_QTGUI_INCLUDE_DIR};${QT_QTCORE_INCLUDE_DIR}")

target_link_libraries(qamp-qt5 ${Qt5Widgets_LIBRARIES} ${LV2_LDFLAGS})
target_link_libraries(qamp-qt4 ${QT_QTGUI_LIBRARY} ${QT_CORE_LIBRARY} ${LV2_LDFLAGS})
target_link_libraries(qamp ${LV2_LDFLAGS})

install(TARGETS qamp-qt5 qamp-qt4 qamp
        DESTINATION lib64/lv2/qamp.lv2)

install(FILES qamp.ttl manifest.ttl
        DESTINATION lib64/lb2/qamp.lv2)


