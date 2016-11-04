#include "d_image_size.h"
#include "ui_d_image_size.h"

D_image_size::D_image_size(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::D_image_size)
{
    ui->setupUi(this);
}

void D_image_size::setWidth(double value)
{
    width = value;
    ui->LE_width->setText(QString("%1").arg(width));
}

void D_image_size::setHeight(double value)
{
    height = value;
    ui->LE_height->setText(QString("%1").arg(height));
}

double D_image_size::getWidth() const
{
    return width;
}


double D_image_size::getHeight() const
{
    return height;
}

D_image_size::~D_image_size()
{
    delete ui;
}

void D_image_size::on_buttonBox_accepted()
{
    height = ui->LE_height->text().toDouble();
    width = ui->LE_width->text().toDouble();
}
