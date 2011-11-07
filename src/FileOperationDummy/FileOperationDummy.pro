# Project type
TEMPLATE = lib

# Global settings. Root variable MUST be present!
Root = ../..
include ($${Root}/config.pri)

# Input
HEADERS += *.h ../IFileOperation.h
SOURCES += *.cpp
FORMS += *.ui