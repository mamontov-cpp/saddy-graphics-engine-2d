#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T10:00:13
#
#-------------------------------------------------

QT       += core
TARGET = is580
TEMPLATE = app

CONFIG += console


SOURCES += main.cpp

DESTDIR = ../../bin/

unix {
        DEFINES += "UNIX=1"
        DEFINES += "LINUX=1"
        DEFINES += "GCC=1"
}

win32 {
        DEFINES += "WIN32=1"
        DEFINES +=  "MINGW=1"
}


CONFIG(debug, debug|release) {
    TARGET = is580-debug
    
}

CONFIG(release, debug|release) {
    TARGET = is580-release    
}

QMAKE_CXXFLAGS += -Wno-reorder -Wno-unused -Wno-sign-compare -w
