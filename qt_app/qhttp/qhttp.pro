QT       += core network
QT       -= gui

TARGET    = qhttp
TEMPLATE  = lib

CONFIG(debug, debug|release) {
    DESTDIR = ../bin/debug
} else
{
    DESTDIR = ../bin/release
}

LIBS  += -L$$DESTDIR

# comment following line to build the lib as static library
#DEFINES *= QHTTP_DYNAMIC_LIB
# comment following line to trim client classes from build
DEFINES *= QHTTP_HAS_CLIENT
# Qt5.5.1 on OSX needs both c++11 and c++14!! the c++14 is not enough
CONFIG  += c++11 c++14

unix{
CONFIG += staticlib
}

PRJDIR    = ..
#include($$PRJDIR/commondir.pri)

INCLUDEPATH +=  . $$PRJDIR/src $$PRJDIR/3rdparty
DEFINES       *= QHTTP_MEMORY_LOG=0
win32:DEFINES *= QHTTP_EXPORT

# Joyent http_parser
SOURCES  += http-parser/http_parser.c
HEADERS  += http-parser/http_parser.h

QMAKE_CFLAGS_RELEASE -= -O
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE *= -O3

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

SOURCES  += \
    qhttpabstracts.cpp \
    qhttpserverconnection.cpp \
    qhttpserverrequest.cpp \
    qhttpserverresponse.cpp \
    qhttpserver.cpp\
	qthreadpoolhttpserver.cpp


HEADERS  += \
    qhttpfwd.hpp \
    qhttpabstracts.hpp \
    qhttpserverconnection.hpp \
    qhttpserverrequest.hpp \
    qhttpserverresponse.hpp \
    qhttpserver.hpp\
	qthreadpoolhttpserver.hpp


contains(DEFINES, QHTTP_HAS_CLIENT) {
    SOURCES += \
        qhttpclientrequest.cpp \
        qhttpclientresponse.cpp \
        qhttpclient.cpp

    HEADERS += \
        qhttpclient.hpp \
        qhttpclientresponse.hpp \
        qhttpclientrequest.hpp
}
