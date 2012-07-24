#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT       += core network gui

TARGET = client
TEMPLATE = app
win32:DEFINES += "WINVER=0x0500"

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    mainwindow.cpp \
    Client.cpp \
    mod_telnet.cpp \
    mod_graph.cpp \
    mod_file.cpp

HEADERS += \
    Client.h \
    mainwindow.h \
    defines.h \
    mod_telnet.h \
    mod_file.h \
    utils.h \
    mod_graph.h


FORMS    += mainwindow.ui
