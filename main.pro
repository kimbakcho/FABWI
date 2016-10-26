#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:55:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

TRANSLATIONS += lang_ko.ts lang_zh.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    idix_gui.cpp \
    e2r_every_report.cpp \
    eismain.cpp

HEADERS  += mainwindow.h \
    idix_gui.h \
    e2r_every_report.h \
    eismain.h

FORMS    += mainwindow.ui \
    idix_gui.ui \
    e2r_every_report.ui \
    eismain.ui

RESOURCES += \
    resourec.qrc
