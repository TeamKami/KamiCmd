# Project type
TEMPLATE = app
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
HEADERS += *.h
SOURCES += *.cpp

#Windows
win32:RC_FILE = KamiCmd.rc
#Mac OS X
ICON = KamiCmd.icns