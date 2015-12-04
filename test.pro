#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T15:26:30
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    ser.h

FORMS    += mainwindow.ui \
    configdialog.ui
