#-------------------------------------------------
#
# Project created by QtCreator 2012-07-08T19:57:25
#
#-------------------------------------------------

QT       += core network

TARGET = client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Client.cpp \
    Application.cpp \
    mod_telnet.cpp \
    mod_graph.cpp

HEADERS += \
    Client.h \
    Application.h \
    defines.h \
    mod_telnet.h \
    mod_graph.h
