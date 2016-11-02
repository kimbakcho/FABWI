#include "d_attachment.h"
#include "ui_d_attachment.h"

D_attachment::D_attachment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::D_attachment)
{
    ui->setupUi(this);
}

D_attachment::~D_attachment()
{
    delete ui;
}
