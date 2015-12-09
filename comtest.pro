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
    myworker.cpp \
    myworker2.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    ser.h \
    myworker.h \
    myworker2.h

FORMS    += mainwindow.ui \
    configdialog.ui

RESOURCES += \
    icon.qrc

RC_FILE = res.rc

