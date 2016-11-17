#include "eis_input_information.h"
#include "ui_eis_input_information.h"

EIS_input_information::EIS_input_information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EIS_input_information)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("EISDB");
    if(!db.isOpen()){
        if(!db.open()){
             int ret = QMessageBox::warning(this, tr("conntion false"),
                                               "server connection fail\n"
                                                  ""+db.lastError().text(),
                                                    QMessageBox::Close);
        }
    }
    init_combo_box();

}

void EIS_input_information::init_combo_box()
{

    QString query_txt;
    QSqlQuery query(db);
    query_txt = QString("select team from People_information group by team order by team asc");
    query.exec(query_txt);
    ui->add_process_team->clear();
    ui->add_process_team->addItem("");
    ui->add_facilities_team->clear();
    ui->add_facilities_team->addItem("");
    ui->add_people_team->clear();
    ui->add_people_team->addItem("");
    while(query.next()){
        ui->add_process_team->addItem(query.value("team").toString());
        ui->add_facilities_team->addItem(query.value("team").toString());
        ui->add_people_team->addItem(query.value("team").toString());
    }

    query_txt = QString("select factory_process from People_information group by factory_process order by factory_process asc");
    query.exec(query_txt);
    ui->add_facilities_process->clear();
    ui->add_facilities_process->addItem("");
    while(query.next()){
        ui->add_facilities_process->addItem(query.value("factory_process").toString());
    }
}

EIS_input_information::~EIS_input_information()
{
    delete ui;
}

void EIS_input_information::on_add_team_btn_clicked()
{
    QSqlQuery query(db);
    QString query_txt = QString("INSERT INTO `People_information` (`team`) VALUES ('%1');").arg(ui->add_team->text());
    query.exec(query_txt);
    init_combo_box();
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);
    msg.setText(tr("add complete"));
    msg.exec();
}

void EIS_input_information::on_add_process_btn_clicked()
{

    QSqlQuery query(db);
    QString query_txt = QString("INSERT INTO `People_information` (`team`, `factory_process`) VALUES ('%1', '%2');")
                                .arg(ui->add_process_team->currentText()).arg(ui->add_process->text());
    query.exec(query_txt);
    init_combo_box();
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);
    msg.setText(tr("add complete"));
    msg.exec();
}

void EIS_input_information::on_add_facilities_btn_clicked()
{
    QSqlQuery query(db);
    QString query_txt = QString("INSERT INTO `FAB`.`People_information` (`team`, `factory_process`, `facilities`) VALUES ('%1', '%2', '%3');")
                                .arg(ui->add_facilities_team->currentText()).arg(ui->add_facilities_process->currentText()).arg(ui->add_facilities->text());
    query.exec(query_txt);
    init_combo_box();
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);
    msg.setText(tr("add complete"));
    msg.exec();
}

void EIS_input_information::on_add_people_btn_clicked()
{
    QSqlQuery query(db);
    QString query_txt = QString("INSERT INTO `People_information` (`team`, `name`) VALUES ('%1', '%2');")
                                .arg(ui->add_people_team->currentText()).arg(ui->add_people->text());
    query.exec(query_txt);
    init_combo_box();
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);
    msg.setText(tr("add complete"));
    msg.exec();
}

