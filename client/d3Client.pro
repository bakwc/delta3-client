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
    UI/mainwindow.cpp \
    client.cpp \
    mod_telnet.cpp \
    mod_graph.cpp \
    mod_abstract.cpp \
    mod_proxy.cpp \
    mod_file.cpp \
    graphmodvideo.cpp

HEADERS += \
    client.h \
    UI/mainwindow.h \
    defines.h \
    mod_telnet.h \
    mod_graph.h \
    utils.h \
    mod_abstract.h \
    mod_proxy.h \
    mod_file.h \
    graphmodvideo.h


FORMS    += UI/mainwindow.ui

unix:LIBS += -lX11 -lXtst

# FFMPEG
DEFINES += __STDC_CONSTANT_MACROS
LIBS += -lavcodec -lavformat -lavutil -lswscale
