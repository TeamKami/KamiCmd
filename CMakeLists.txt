cmake_minimum_required(VERSION 2.6)
project(KamiCmd)

set(TARGET_NAME KamiCmd)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# ************
# PACKAGE VERSION
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "50")
# ************

include(Internal)
include(qt4_wrap_ts)
include(KamiCmdModuleDev)


find_package (Qt4 REQUIRED)
set(QT_USE_QTGUI TRUE)
set(QT_USE_QTWEBKIT TRUE)
set(QT_USE_QPLUGIN TRUE)
include (${QT_USE_FILE})

if (WIN32)
set(CMAKE_EXTRA_CXX_FLAGS "/Zc:wchar_t-" CACHE STRING "Extra CXX options")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_EXTRA_CXX_FLAGS}")

add_definitions(
	-DUNICODE
)

add_subdirectory(3rdparty)


include_directories(${3RDPARTY_INCLUDES})

add_subdirectory(src)
