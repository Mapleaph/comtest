#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T15:26:30
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configdialog.cpp \
    myworker.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    ser.h \
    myworker.h

FORMS    += mainwindow.ui \
    configdialog.ui

RESOURCES += \
    res.qrc

RC_FILE = res.rc

ICON = icon.icns

VERSION = 2.2.0.1

TRANSLATIONS = cn.ts
