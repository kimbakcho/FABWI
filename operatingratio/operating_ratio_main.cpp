#include "operating_ratio_main.h"
#include "ui_operating_ratio_main.h"

operating_ratio_main::operating_ratio_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::operating_ratio_main)
{
    ui->setupUi(this);
}

operating_ratio_main::~operating_ratio_main()
{
    delete ui;
}

void operating_ratio_main::on_action_btn_clicked()
{
    operating_ratio_action * action_from = new operating_ratio_action();
    action_from->show();
}
