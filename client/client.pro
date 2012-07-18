#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT       += core network gui

TARGET = client
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    mainwindow.cpp \
    Client.cpp \
    mod_telnet.cpp \
    mod_graph.cpp

HEADERS += \
    Client.h \
    mainwindow.h \
    defines.h \
    mod_telnet.h \
    mod_graph.h \
    utils.h


FORMS    += mainwindow.ui

OTHER_FILES += \
    delta3.png
