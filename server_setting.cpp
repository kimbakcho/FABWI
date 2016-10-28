#include "server_setting.h"
#include "ui_server_setting.h"

Server_setting::Server_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server_setting)
{
    ui->setupUi(this);
    QString configini_str = "server.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    ui->LE_server_ip->setText(settings.value("serverip").toString());
    ui->LE_server_dbport->setText(settings.value("dbport").toString());
    ui->LE_server_ftpport->setText(settings.value("ftpport").toString());
    settings.endGroup();
}

Server_setting::~Server_setting()
{
    delete ui;
}

void Server_setting::closeEvent(QCloseEvent *)
{
    this->deleteLater();
}

void Server_setting::on_buttonBox_accepted()
{
    QString configini_str = "server.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    settings.setValue("serverip",ui->LE_server_ip->text());
    settings.setValue("dbport",ui->LE_server_dbport->text());
    settings.setValue("ftpport",ui->LE_server_ftpport->text());
    settings.endGroup();
}

void Server_setting::on_buttonBox_rejected()
{
    this->close();
}
