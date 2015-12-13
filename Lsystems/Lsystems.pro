#-------------------------------------------------
#
# Project created by QtCreator 2015-12-10T17:46:34
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lsystems
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    lsystem.cpp

HEADERS  += mainwindow.h \
    lsystem.h

FORMS    += mainwindow.ui
