#include "nikon_log_err_list_item.h"
#include "ui_nikon_log_err_list_item.h"

nikon_log_err_list_item::nikon_log_err_list_item(QString bar_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nikon_log_err_list_item)
{
    ui->setupUi(this);
    LA_data_name = ui->data_name;
    LA_data_count = ui->data_count;
    Cb_check_box = ui->checkBox;
    LA_data_color = ui->data_color;
    bar_data = new QBarSet(bar_name);
    bar_data->setLabel(bar_name);
    linedata= new QLineSeries();
    linedata->setName(bar_name);

    connect(bar_data,SIGNAL(hovered(bool,int)),this,SLOT(slot_barset_hover(bool,int)));

}

QString nikon_log_err_list_item::getSource_data() const
{
    return source_data;
}

void nikon_log_err_list_item::setSource_data(const QString &value)
{
    source_data = value;
}

QLineSeries *nikon_log_err_list_item::getLinedata() const
{
    return linedata;
}

void nikon_log_err_list_item::setLinedata(QLineSeries *value)
{
    linedata = value;
}

QBarSet *nikon_log_err_list_item::getBar_data() const
{
    return bar_data;
}

void nikon_log_err_list_item::setBar_data(QBarSet *value)
{
    bar_data = value;
}

void nikon_log_err_list_item::slot_barset_hover(bool status, int index)
{
    if(status){
        emit sig_hover_change(ui->data_name->text());
    }
}

QLabel *nikon_log_err_list_item::getLA_data_color() const
{
    return LA_data_color;
}

void nikon_log_err_list_item::setLA_data_color(QLabel *value)
{
    LA_data_color = value;
}

QCheckBox *nikon_log_err_list_item::getCb_check_box() const
{
    return Cb_check_box;
}

void nikon_log_err_list_item::setCb_check_box(QCheckBox *value)
{
    Cb_check_box = value;
}

QLabel *nikon_log_err_list_item::getLA_data_count() const
{
    return LA_data_count;
}

void nikon_log_err_list_item::setLA_data_count(QLabel *value)
{
    LA_data_count = value;
}

QLabel *nikon_log_err_list_item::getLA_data_name() const
{
    return LA_data_name;
}

void nikon_log_err_list_item::setLA_data_name(QLabel *value)
{
    LA_data_name = value;
}

void nikon_log_err_list_item::set_dataname(QString str)
{
    ui->data_name->setText(str);
}

void nikon_log_err_list_item::set_count_data(QString str)
{
    ui->data_count->setText(str);
}

nikon_log_err_list_item::~nikon_log_err_list_item()
{
//    bar_data->deleteLater();
    delete ui;
}


void nikon_log_err_list_item::on_checkBox_toggled(bool checked)
{

}
