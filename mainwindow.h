#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <idix_gui.h>
#include <e2r_every_report.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <server_setting.h>
#include "Smtp/SmtpMime"
#include <logsrc/etching_analysor.h>
#include <logsrc/nikon_log_err_main.h>
#include <logsrc/hanile_log_main.h>
#include <temp_humi_widget.h>
#include <logsrc/asml_err_log_main.h>
#include <worksupport/worst_search_excel_main.h>
#include <logsrc/nikon_time_log.h>
#include <operatingratio/operating_ratio_main.h>
#include <bridge_chart/bridge_chart_widget.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString testhagl;

    ~MainWindow();

private slots:
    void on_action_everyday_report_triggered();

    void on_actionEISystem_triggered();

    void on_actionMessage_triggered();

    void on_action_server_setting_triggered();

    void on_infrom_entet_btn_clicked();

//    void on_testbtn_clicked();

    void mail_contionfail(QByteArray msg);
    void mail_fail(int a,int b,QByteArray msg);

    void on_action_etching_analysor_triggered();

    void on_action_nikon_err_log_triggered();

    void on_action_hanile_triggered();

    void on_action_EVATEC_triggered();

    void on_action_temp_humi_triggered();

    void on_action_asml_err_triggered();

    void on_action_worst_serarch_triggered();

    void on_OI_startup_clicked();

    void on_action_nikon_err_time_triggered();

    void on_actionMES_bridge_chart_triggered();

private:
    void closeEvent(QCloseEvent *event);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
