#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT += core network gui

TARGET = delta3client
TEMPLATE = lib
DESTDIR = ..
win32:DEFINES += "WINVER=0x0500"

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    client.cpp \
    mod_telnet.cpp \
    mod_graph.cpp \
    mod_abstract.cpp

HEADERS += \
    client.h \
    defines.h \
    mod_telnet.h \
    mod_graph.h \
    utils.h \
    mod_abstract.h
