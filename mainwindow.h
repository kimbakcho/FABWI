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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString testhagl;
    QNetworkAccessManager *manager;
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

private:
    void closeEvent(QCloseEvent *event);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
