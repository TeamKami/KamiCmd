CONFIG += qt thread #wwwidgets

CONFIG(debug, debug|release)
{ 
    contains(TEMPLATE, vclib) | contains(TEMPLATE, lib) {
        DESTDIR = $${Root}/debug/Modules
    } else {
        DESTDIR = $${Root}/debug
    }
#   LIBS += "$$[QT_INSTALL_BINS]/wwwidgets4d.lib"
}
CONFIG(release, debug|release)
{ 
    contains(TEMPLATE, vclib) | contains(TEMPLATE, lib) {
        macx {
            DESTDIR = $${Root}/release/KamiCmd.app/Contents/MacOS
        }
        else {
            DESTDIR = $${Root}/release/Modules
        }
    } else {
        DESTDIR = $${Root}/release
    }
#   LIBS += "$$[QT_INSTALL_BINS]/wwwidgets4.lib"
}
INCLUDEPATH += \ #$${Root}/3rdparty/wwWidgets/include \
               $${Root}/src
