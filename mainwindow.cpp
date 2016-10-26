#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <eismain.h>
#include <e2r_every_report.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_everyday_report_triggered()
{
    E2R_every_report *report = new E2R_every_report();
    report->show();
}

void MainWindow::on_actionEISystem_triggered()
{
    EISmain *eismain = new EISmain();
    eismain->show();
}

void MainWindow::on_actionMessage_triggered()
{
//    QNetworkRequest sendformat;
//    QUrlQuery postData;
//    postData.addQueryItem("user", "vngkgk624");
//    postData.addQueryItem("password", "super624");
//    postData.addQueryItem("to", "01040988663");
//    postData.addQueryItem("from", "01040988663");
//    postData.addQueryItem("text", "hello");

//    sendformat.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

//    sendformat.setUrl(QUrl("http://api.coolsms.co.kr/sendmsg"));

//    manager->post(sendformat,postData.toString(QUrl::FullyEncoded).toUtf8());


}
