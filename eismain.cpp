#include "eismain.h"
#include "ui_eismain.h"

EISmain::EISmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EISmain)
{
    ui->setupUi(this);
}

EISmain::~EISmain()
{
    delete ui;
}
