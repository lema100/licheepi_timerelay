QT = core network

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CFLAGS_RELEASE = -O3
QMAKE_CXXFLAGS_RELEASE = -O3

CONFIG(debug, debug|release) {
	DESTDIR = ../bin/debug
	LIBS += -L$$DESTDIR -lqhttp
} else
{
	DESTDIR = ../bin/release
	LIBS += -L$$DESTDIR -lqhttp
}


include(qt_app.pri)
