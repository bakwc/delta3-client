#-------------------------------------------------
#
# Project created by QtCreator 2012-07-15T12:24:48
#
#-------------------------------------------------

QT       += core network
QT -= gui

TARGET = d3cDesktop
TEMPLATE = app
INCLUDEPATH += ../client
win32:DEFINES += "WINVER=0x0500"

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \

HEADERS += \


LIBS += -L.. -ldelta3client
