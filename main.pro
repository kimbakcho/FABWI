#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:55:00
#
#-------------------------------------------------

QT       += core gui network ftp sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

LIBS += -lSMTPEmail

TRANSLATIONS += lang_ko.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    idix_gui.cpp \
    e2r_every_report.cpp \
    eismain.cpp \
    btextedit.cpp \
    server_setting.cpp \
    d_image_size.cpp \
    d_attachment.cpp \
    eis_big_view.cpp \
    eis_serarch_item.cpp \
    eis_listview_item.cpp \
    eis_list_view.cpp \
    b_image_resize_widget.cpp


HEADERS  += mainwindow.h \
    idix_gui.h \
    e2r_every_report.h \
    eismain.h \
    btextedit.h \
    server_setting.h \
    d_image_size.h \
    d_attachment.h \
    eis_big_view.h \
    eis_serarch_item.h \
    eis_listview_item.h \
    eis_list_view.h \
    global_define.h \
    b_image_resize_widget.h


FORMS    += mainwindow.ui \
    idix_gui.ui \
    e2r_every_report.ui \
    eismain.ui \
    server_setting.ui \
    d_image_size.ui \
    d_attachment.ui \
    eis_big_view.ui \
    eis_serarch_item.ui \
    eis_listview_item.ui \
    eis_list_view.ui \
    b_image_resize_widget.ui

RESOURCES += \
    resourec.qrc
