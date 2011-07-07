# Project type
TEMPLATE = lib
#QT += opengl

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
LIBS += -L../../release
}

LIBS += -lqscintilla2
INCLUDEPATH += ../../3rdparty/QScintilla/QScintilla2
