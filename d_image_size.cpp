#include "d_image_size.h"
#include "ui_d_image_size.h"

D_image_size::D_image_size(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::D_image_size)
{
    ui->setupUi(this);
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
