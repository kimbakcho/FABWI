#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:55:00
#
#-------------------------------------------------

QT       += core gui network ftp sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

TRANSLATIONS += lang_ko.ts

QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO

RC_FILE = myapp.rc

LIBS += -lSMTPEmail

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
    b_image_resize_widget.cpp \
    eis_alarm_serarch_item.cpp \
    eis_alarmlistview_item.cpp \
    eis_delete_dialog.cpp \
    eis_input_information.cpp \
    etching_analysor.cpp \
    etching_chart_1.cpp \
    etching_chart_1_view.cpp \
    etching_base_chart_view.cpp \
    etching_chart_bigview.cpp \
    etching_analysor_item.cpp \
    eis_select_copymode_dialog.cpp


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
    b_image_resize_widget.h \
    eis_alarm_serarch_item.h \
    eis_alarmlistview_item.h \
    eis_delete_dialog.h \
    eis_input_information.h \
    etching_analysor.h \
    etching_chart_1.h \
    etching_chart_1_view.h \
    etching_base_chart_view.h \
    etching_chart_bigview.h \
    etching_analysor_item.h \
    eis_select_copymode_dialog.h


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
    b_image_resize_widget.ui \
    eis_alarm_serarch_item.ui \
    eis_alarmlistview_item.ui \
    eis_delete_dialog.ui \
    eis_input_information.ui \
    etching_analysor.ui \
    etching_base_chart_view.ui \
    etching_chart_bigview.ui \
    etching_analysor_item.ui \
    eis_select_copymode_dialog.ui

RESOURCES += \
    resourec.qrc
