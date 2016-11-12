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
    SmtpClient smtp("mail.wisol.co.kr", 110, SmtpClient::SslConnection);

}

void MainWindow::on_action_server_setting_triggered()
{
    Server_setting *setting_log = new Server_setting();
    setting_log->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString temp_dir_path = qApp->applicationDirPath()+"/temp/EIS/img/";
    QDir tempdir(temp_dir_path);
    QStringList dir_list = tempdir.entryList();
    for(int i=0;i<dir_list.size();i++){
        if(dir_list.at(i) != "." && dir_list.at(i) != ".."){
             QString rm_dir_path = temp_dir_path+ dir_list.at(i)+"/";
             QDir rm_dir(rm_dir_path);
             for(int j=0;j<rm_dir.entryList().size();j++){
                if(rm_dir.entryList().at(j) != "." && rm_dir.entryList().at(j) != ".."){
                    rm_dir.remove(rm_dir.entryList().at(j));
                }
             }
             tempdir.rmdir(dir_list.at(i));
        }
    }

}

void MainWindow::on_infrom_entet_btn_clicked()
{
    EISmain *eismain = new EISmain();
    eismain->show();
}

void MainWindow::on_testbtn_clicked()
{

}

