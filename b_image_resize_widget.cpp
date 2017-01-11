#include "b_image_resize_widget.h"
#include "ui_b_image_resize_widget.h"

B_image_resize_widget::B_image_resize_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::B_image_resize_widget)
{
    ui->setupUi(this);
    ready_flag = false;
    size_edit_mode =false;
    mouse_press_flag = false;
}

bool B_image_resize_widget::getSize_edit_mode() const
{
    return size_edit_mode;
}

void B_image_resize_widget::setSize_edit_mode(bool value)
{
    size_edit_mode = value;
}

QLineEdit *B_image_resize_widget::getLE_height() const
{
    return LE_height;
}

void B_image_resize_widget::setLE_height(QLineEdit *value)
{
    LE_height = value;
}

QLineEdit *B_image_resize_widget::getLE_width() const
{
    return LE_width;
}

void B_image_resize_widget::setLE_width(QLineEdit *value)
{
    LE_width = value;
}

QString B_image_resize_widget::getFilepath() const
{
    return filepath;
}

bool B_image_resize_widget::getReady_flag() const
{
    return ready_flag;
}

void B_image_resize_widget::setReady_flag(bool value)
{
    ready_flag = value;
}

void B_image_resize_widget::paintEvent(QPaintEvent *event)
{
    if(ready_flag ){
        QPainter painter(this);
        QRect r(0,0,width(),height());
        if(!size_edit_mode){
            LE_width->setText(QString("%1").arg(width()));
            LE_height->setText(QString("%1").arg(height()));
        }
        painter.drawImage(r,img);
    }
}

void B_image_resize_widget::setFilepath(const QString &value)
{
    filepath = value;
    img.load(value);
    ready_flag = true;
    update();
}

QImage B_image_resize_widget::getImg() const
{
    return img;
}

void B_image_resize_widget::setImg(const QImage &value)
{
    img = value;
    ready_flag = true;
    update();
}

B_image_resize_widget::~B_image_resize_widget()
{
    delete ui;
}

void B_image_resize_widget::resizeEvent(QResizeEvent *event)
{
    update();
}
