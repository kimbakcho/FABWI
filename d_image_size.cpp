#include "d_image_size.h"
#include "ui_d_image_size.h"

D_image_size::D_image_size(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::D_image_size)
{
    ui->setupUi(this);
    this->filepath  = filepath;
    ui->reszie_windows->subWindowList().at(0)->setWindowFlags(Qt::WindowTitleHint);
    ui->subwindow->setLE_width(ui->LE_width);
    ui->subwindow->setLE_height(ui->LE_height);
}

QImage D_image_size::getImg() const
{
    return img;
}

void D_image_size::setImg(const QImage &value)
{
    img = value;
    ui->subwindow->setImg(img);
}

void D_image_size::image_pack()
{
    ui->reszie_windows->subWindowList().at(0)->resize(getWidth(),getHeight());
    resize(getWidth()+30,getHeight()+140);
}

void D_image_size::setFilepath(const QString &value)
{
    filepath = value;
    ui->subwindow->setFilepath(filepath);
}

QString D_image_size::getFilepath() const
{
    return filepath;
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

void D_image_size::on_size_edit_toggled(bool checked)
{
      ui->subwindow->setSize_edit_mode(checked);
      if(checked){
        ui->LE_height->setReadOnly(false);
        ui->LE_width->setReadOnly(false);
      }else {
        ui->LE_height->setReadOnly(true);
        ui->LE_width->setReadOnly(true);
      }
}

void D_image_size::on_LE_height_editingFinished()
{

}

void D_image_size::on_LE_width_editingFinished()
{
    if(ui->subwindow->getSize_edit_mode()){
        height = ui->LE_height->text().toDouble();
        ui->reszie_windows->subWindowList().at(0)->resize(width,height);
//        resize(getWidth()+30,getHeight()+140);

    }
}

void D_image_size::on_LE_height_textEdited(const QString &arg1)
{
    if(ui->subwindow->getSize_edit_mode()){
        height = arg1.toDouble();
        ui->reszie_windows->subWindowList().at(0)->resize(width,height);
//        resize(getWidth()+30,getHeight()+140);
    }
}

void D_image_size::on_LE_width_textEdited(const QString &arg1)
{
    if(ui->subwindow->getSize_edit_mode()){
        width = arg1.toDouble();
        ui->reszie_windows->subWindowList().at(0)->resize(width,height);
//        resize(getWidth()+30,getHeight()+140);
    }
}
