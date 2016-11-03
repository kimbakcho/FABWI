#include "eis_serarch_item.h"
#include "ui_eis_serarch_item.h"

EIS_serarch_item::EIS_serarch_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EIS_serarch_item)
{
    ui->setupUi(this);
}

EIS_serarch_item::~EIS_serarch_item()
{
    delete ui;
}
