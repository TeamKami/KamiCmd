# Project type
TEMPLATE = lib
greaterThan(QT_MAJOR_VERSION, 4) {
        QT += widgets
        QT += quick1
    } else {
        QT += declarative
    }

# Global settings. Root variable MUST be present!
Root = ../..
include ($${Root}/config.pri)

# Input
HEADERS += *.h ../IFileOperation.h
SOURCES += *.cpp
FORMS += *.ui