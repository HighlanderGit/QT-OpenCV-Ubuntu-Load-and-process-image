#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T14:46:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proyecto2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


INCLUDEPATH += "/usr/local/include/"

LIBS += `pkg-config --libs opencv`
