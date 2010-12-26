# Project type
TEMPLATE = lib

# Global settings. Root variable MUST be present!
Root = ../..
include ($${Root}/config.pri)

# Input
HEADERS += *.h
FORMS += *.ui
SOURCES += *.cpp
RESOURCE += *.qrc
