# Project type
TEMPLATE = lib
#QT += opengl

# Global settings. Root variable MUST be present!
Root = ../..
include ($${Root}/config.pri)

# Input
HEADERS += *.h
SOURCES += *.cpp
FORMS += *.ui

CONFIG(debug, debug|release)
{
LIBS += -L../../debug
}
CONFIG(release, debug|release)
{
LIBS += -L../../release
}

LIBS += -lqscintilla2
INCLUDEPATH += ../../3rdparty/QScintilla/QScintilla2
