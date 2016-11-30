#include "asml_err_log_list_item.h"
#include "ui_asml_err_log_list_item.h"

ASML_err_log_list_item::ASML_err_log_list_item(QString bar_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ASML_err_log_list_item)
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

QString ASML_err_log_list_item::getExplan() const
{
    return explan;
}

void ASML_err_log_list_item::setExplan(const QString &value)
{
    explan = value;
}

void ASML_err_log_list_item::set_dataname(QString str)
{
    ui->data_name->setText(str);
}

void ASML_err_log_list_item::set_count_data(QString str)
{
    ui->data_count->setText(str);
}

QString ASML_err_log_list_item::getSource_data() const
{
    return source_data;
}

void ASML_err_log_list_item::setSource_data(const QString &value)
{
    source_data = value;
}

void ASML_err_log_list_item::slot_barset_hover(bool status, int index)
{
    if(status){
        emit sig_hover_change(ui->data_name->text(),explan);
    }
}

void ASML_err_log_list_item::on_checkBox_toggled(bool checked)
{

}

QLineSeries *ASML_err_log_list_item::getLinedata() const
{
    return linedata;
}

void ASML_err_log_list_item::setLinedata(QLineSeries *value)
{
    linedata = value;
}

QBarSet *ASML_err_log_list_item::getBar_data() const
{
    return bar_data;
}

void ASML_err_log_list_item::setBar_data(QBarSet *value)
{
    bar_data = value;
}

QCheckBox *ASML_err_log_list_item::getCb_check_box() const
{
    return Cb_check_box;
}

void ASML_err_log_list_item::setCb_check_box(QCheckBox *value)
{
    Cb_check_box = value;
}

QLabel *ASML_err_log_list_item::getLA_data_color() const
{
    return LA_data_color;
}

void ASML_err_log_list_item::setLA_data_color(QLabel *value)
{
    LA_data_color = value;
}

QLabel *ASML_err_log_list_item::getLA_data_count() const
{
    return LA_data_count;
}

void ASML_err_log_list_item::setLA_data_count(QLabel *value)
{
    LA_data_count = value;
}

QLabel *ASML_err_log_list_item::getLA_data_name() const
{
    return LA_data_name;
}

void ASML_err_log_list_item::setLA_data_name(QLabel *value)
{
    LA_data_name = value;
}

ASML_err_log_list_item::~ASML_err_log_list_item()
{
    delete ui;
}
