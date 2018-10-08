#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T16:23:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoSift
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    exif.cpp \
    gps.cpp \
    easytools.cpp \
    MyThread.cpp \
    GPSData.cpp

HEADERS  += mainwindow.h \
    exif.h \
    gps.h \
    easytools.h \
    MyThread.h \
    GPSData.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc

RC_FILE += ico.rc


