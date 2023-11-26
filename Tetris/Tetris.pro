#-------------------------------------------------
#
# Project created by QtCreator 2015-11-01T22:56:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tetris
TEMPLATE = app


SOURCES += main.cpp\
    tetrisitem.cpp \
        tetrisview.cpp \
    tetrismodel.cpp \
    tetriscontroller.cpp

HEADERS  += tetrisview.h \
    tetrisitem.h \
    tetrismodel.h \
    tetriscontroller.h

FORMS    +=

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    iconImage.qrc
