QT = core network concurrent

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) {
    DESTDIR = ../bin/debug
} else
{
    DESTDIR = ../bin/release
}

LIBS += -L$$DESTDIR -lqhttp

include(qt_app.pri)
