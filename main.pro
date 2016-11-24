#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T09:55:00
#
#-------------------------------------------------

QT       += core gui network ftp sql printsupport charts datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

TRANSLATIONS += lang_ko.ts

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
    eis_select_copymode_dialog.cpp \
    logsrc/etching_analysor.cpp \
    logsrc/etching_analysor_item.cpp \
    logsrc/etching_base_chart_view.cpp \
    logsrc/etching_chart_1.cpp \
    logsrc/etching_chart_1_view.cpp \
    logsrc/etching_chart_bigview.cpp \
    logsrc/nikon_log_err_chart.cpp \
    logsrc/nikon_log_err_chartview.cpp \
    logsrc/nikon_log_err_linseries.cpp \
    logsrc/nikon_log_err_list_item.cpp \
    logsrc/nikon_log_err_main.cpp \
    logsrc/nikon_log_err_thread.cpp \
    b_visualization_test.cpp


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
    eis_select_copymode_dialog.h \
    logsrc/etching_analysor.h \
    logsrc/etching_analysor_item.h \
    logsrc/etching_base_chart_view.h \
    logsrc/etching_chart_1.h \
    logsrc/etching_chart_1_view.h \
    logsrc/etching_chart_bigview.h \
    logsrc/nikon_log_err_chart.h \
    logsrc/nikon_log_err_chartview.h \
    logsrc/nikon_log_err_linseries.h \
    logsrc/nikon_log_err_list_item.h \
    logsrc/nikon_log_err_main.h \
    logsrc/nikon_log_err_thread.h \
    b_visualization_test.h


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
    eis_select_copymode_dialog.ui \
    logsrc/etching_analysor.ui \
    logsrc/etching_analysor_item.ui \
    logsrc/etching_base_chart_view.ui \
    logsrc/etching_chart_bigview.ui \
    logsrc/nikon_log_err_list_item.ui \
    logsrc/nikon_log_err_main.ui \
    b_visualization_test.ui

RESOURCES += \
    resourec.qrc
