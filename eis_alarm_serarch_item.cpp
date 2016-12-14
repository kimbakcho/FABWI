#include "eis_alarm_serarch_item.h"
#include "ui_eis_alarm_serarch_item.h"

eis_alarm_serarch_item::eis_alarm_serarch_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eis_alarm_serarch_item)
{
    ui->setupUi(this);
}

eis_alarm_serarch_item::~eis_alarm_serarch_item()
{
    delete ui;
}

void eis_alarm_serarch_item::on_attacth_file_btn_clicked()
{

}
