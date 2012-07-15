#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT       += core network gui

TARGET = client
TEMPLATE = app


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
