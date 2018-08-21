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
    about.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    ser.h \
    myworker.h \
    about.h

FORMS    += mainwindow.ui \
    configdialog.ui \
    about.ui

RESOURCES += \
    res.qrc

ICON = icon.icns

RC_ICONS = icon.ico

VERSION = 2.2.1.3

QMAKE_TARGET_DESCRIPTION = "Serial Port Test Tool"

DEFINES += VERSION_STRING=\\\"$$VERSION\\\"

DEFINES += BUILD_DATE_STRING=\\\"20180821\\\"

TRANSLATIONS += cn.ts \
                en.ts
