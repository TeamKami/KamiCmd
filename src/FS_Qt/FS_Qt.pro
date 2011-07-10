# Project type
TEMPLATE = lib
QT = core

# Global settings. Root variable MUST be present!
Root = ../..
include ($${Root}/config.pri)

# Input
HEADERS += *.h
SOURCES += *.cpp
