greaterThan(QT_MAJOR_VERSION, 4) {
        QT += widgets
        QT += quick1
    } else {
        QT += declarative
    }
INCLUDEPATH += 3rdparty/QScintilla/QScintilla2 \
	3rdparty/QScintilla
TEMPLATE = subdirs
SUBDIRS = src\
	3rdparty
src.depends = 3rdparty
