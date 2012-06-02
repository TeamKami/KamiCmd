# Project type
TEMPLATE = lib
#QT += opengl
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
HEADERS += getLexers.h \
	library.h \
	ObjectWrapper.h

SOURCES += getLexers.cpp \
	library.cpp

CONFIG(debug, debug|release)
{
LIBS += -L../../debug
}
CONFIG(release, debug|release)
{
    macx {
        LIBS += -L../../release/KamiCmd.app/Contents/MacOS
    }
    else {
        LIBS += -L../../release
    }
}

LIBS += -lqscintilla2
INCLUDEPATH += ../../3rdparty/QScintilla/QScintilla2
