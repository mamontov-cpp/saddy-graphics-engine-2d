#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T07:55:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atlasgen
TEMPLATE = app


SOURCES += main.cpp\
        texture.cpp\
		texturearray.cpp\
		atlasentry.cpp\
		atlas.cpp\
    reader.cpp \
    xmlreader.cpp

HEADERS  += mainwindow.h \
    atlas.h \
    atlasentry.h \
    texture.h \
    texturearray.h \
    xmlreader.h \
    reader.h

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
    TARGET = atlasgen-debug
    
}

CONFIG(release, debug|release) {
    TARGET = atlasgen-release    
}

QMAKE_CXXFLAGS += -Wno-reorder -Wno-unused -Wno-sign-compare -w
