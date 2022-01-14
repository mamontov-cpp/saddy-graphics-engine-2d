#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T07:55:13
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atlasgen
TEMPLATE = app

CONFIG += console


SOURCES += main.cpp\
        texture.cpp\
		texturearray.cpp\
		atlasentry.cpp\
		atlas.cpp\
    reader.cpp \
    xmlreader.cpp \
    jsonreader.cpp \
    fullsearchpacker/glueentry.cpp \
    fullsearchpacker/glueorder.cpp \
    maybenumber.cpp \
    fullsearchpacker/gluemetric.cpp \
    fullsearchpacker/gluetask.cpp \
    fullsearchpacker/mindiffmetric.cpp \
    fullsearchpacker/minareametric.cpp \
    fullsearchpacker/imagegluingorder.cpp \
    fullsearchpacker/imagearranger.cpp \
	growingbinpacker/growingbinpacker.cpp \
    horizontalpacker.cpp \
    jsonwriter.cpp \
    writer.cpp \
    xmlwriter.cpp \
    packer.cpp \
    fullsearchpacker/fullsearchpacker.cpp 
	

HEADERS  +=  atlas.h \
    atlasentry.h \
    texture.h \
    texturearray.h \
    xmlreader.h \
    reader.h \
    jsonreader.h \
    fullsearchpacker/glueentry.h \
    fullsearchpacker/gluemode.h \
    fullsearchpacker/glueorder.h \
	growingbinpacker/growingbinpacker.h \	
    maybenumber.h \
    fullsearchpacker/gluemetric.h \
    fullsearchpacker/gluetask.h \
    fullsearchpacker/mindiffmetric.h \
    fullsearchpacker/minareametric.h \
    fullsearchpacker/imagegluingorder.h \
    fullsearchpacker/imagearranger.h \
    jsonwriter.h \
    horizontalpacker.h \
    writer.h \
    packer.h \	
    xmlwriter.h \
    outputoptions.h \
    fullsearchpacker/fullsearchpacker.h 
	
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
