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

    QString temp_dir_path2 = qApp->applicationDirPath()+"/temp/EIS/alarmimg/";
    QDir tempdir2(temp_dir_path2);
    QStringList dir_list2 = tempdir2.entryList();
    for(int i=0;i<dir_list2.size();i++){
        if(dir_list2.at(i) != "." && dir_list2.at(i) != ".."){
             QString rm_dir_path = temp_dir_path2+ dir_list2.at(i)+"/";
             QDir rm_dir(rm_dir_path);
             for(int j=0;j<rm_dir.entryList().size();j++){
                if(rm_dir.entryList().at(j) != "." && rm_dir.entryList().at(j) != ".."){
                    rm_dir.remove(rm_dir.entryList().at(j));
                }
             }
             tempdir2.rmdir(dir_list2.at(i));
        }
    }


}

void MainWindow::on_infrom_entet_btn_clicked()
{
    EISmain *eismain = new EISmain();
    eismain->show();
}

//void MainWindow::on_testbtn_clicked()
//{

//    //mail example code
//    SmtpClient smtp("mx.info.wisol.co.kr", 25, SmtpClient::TcpConnection);

//    MimeMessage message;

//    EmailAddress sender("bhkim@wisol.co.kr", "beckho");
//    message.setSender(&sender);

//    EmailAddress to("bhkim@wisol.co.kr", "beckho");
//    message.addRecipient(&to);

//    message.setSubject("SmtpClient for Qt - Demo");
//    MimeHtml html;

//    html.setHtml("<h1> Hello! </h1>"
//                 "<h2> This is the first image </h2>");

//    MimeInlineFile image1 (new QFile("1111.png"));

//    image1.setContentId("image1");
//    image1.setContentType("image/png");


//    message.addPart(&html);
//    message.addPart(&image1);


//    if (!smtp.connectToHost()) {
//        qDebug() << "Failed to connect to host!" << endl;
//    }
//    if (!smtp.sendMail(message)) {
//        qDebug() << "Failed to send mail!" << endl;

//    }
//    smtp.quit();

//}

void MainWindow::mail_contionfail(QByteArray msg)
{
    qDebug()<<msg;
}

void MainWindow::mail_fail(int a, int b, QByteArray msg)
{
    qDebug()<<msg;
}


void MainWindow::on_action_etching_analysor_triggered()
{
    etching_analysor *analysor = new etching_analysor();
    analysor->show();

}

void MainWindow::on_action_nikon_err_log_triggered()
{
    nikon_log_err_main *analysor = new nikon_log_err_main();
    analysor->show();

}

void MainWindow::on_action_hanile_triggered()
{
    hanile_log_main *analysor = new hanile_log_main();
    analysor->show();
}

void MainWindow::on_action_EVATEC_triggered()
{

}

void MainWindow::on_action_temp_humi_triggered()
{
    temp_humi_widget *popup = new temp_humi_widget();
    popup->show();

}

void MainWindow::on_action_asml_err_triggered()
{
    ASML_err_log_main *analysor = new ASML_err_log_main();
    analysor->show();
}

void MainWindow::on_action_worst_serarch_triggered()
{
    worst_search_excel_main *popup = new worst_search_excel_main();
    popup->show();

}

void MainWindow::on_OI_startup_clicked()
{

}
