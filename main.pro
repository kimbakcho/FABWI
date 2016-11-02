#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:55:00
#
#-------------------------------------------------

QT       += core gui network ftp sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

TRANSLATIONS += lang_ko.ts lang_zh.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    idix_gui.cpp \
    e2r_every_report.cpp \
    eismain.cpp \
    btextedit.cpp \
    server_setting.cpp \
    d_image_size.cpp \
    d_attachment.cpp \
    eis_big_view.cpp

HEADERS  += mainwindow.h \
    idix_gui.h \
    e2r_every_report.h \
    eismain.h \
    btextedit.h \
    server_setting.h \
    d_image_size.h \
    d_attachment.h \
    eis_big_view.h

FORMS    += mainwindow.ui \
    idix_gui.ui \
    e2r_every_report.ui \
    eismain.ui \
    server_setting.ui \
    d_image_size.ui \
    d_attachment.ui \
    eis_big_view.ui

RESOURCES += \
    resourec.qrc
