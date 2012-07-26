#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT       += core network gui

TARGET = d3cDesktop
TEMPLATE = app
INCLUDEPATH += ../client
win32:DEFINES += "WINVER=0x0500"

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    UI/mainwindow.cpp \

HEADERS += \
    UI/mainwindow.h \


FORMS    += UI/mainwindow.ui

LIBS += -L.. -ldelta3client
