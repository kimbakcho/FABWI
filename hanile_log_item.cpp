#include "hanile_log_item.h"
#include "ui_hanile_log_item.h"

hanile_log_item::hanile_log_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hanile_log_item)
{
    ui->setupUi(this);
}


QString hanile_log_item::getFile_path() const
{
    return file_path;
}

void hanile_log_item::setFile_path(const QString &value)
{
    file_path = value;
    source_file.setFileName(value);
}


hanile_log_item::~hanile_log_item()
{
    delete ui;
}

bool hanile_log_item::file_read()
{
    source_file.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!source_file.atEnd())
    {
        QString temp_data = source_file.readLine();
        sourceline_list<<temp_data;
        if(temp_data.indexOf("Layer No")>=0){
            hanile_item *item = new hanile_item();
            item->getLA_filename()->setText(getFile_path().split("/").last());
            item->setData_start_point(sourceline_list.count()+1);
            QStringList data_split = temp_data.split(",");
            item->getLA_layernumber()->setText(data_split.at(1));
            QString timedata = data_split.at(3);
            QString date = timedata.split(" ").at(0);
            QString year = date.split("_").at(0);
            QString month = date.split("_").at(1);
            QString day = date.split("_").at(2);

            item->date_time.setDate(QDate(year.toInt()
                                               ,month.toInt()
                                               ,day.toInt()
                                               ));
            QString time = timedata.split(" ").at(1);
            QString hour = time.split("_").at(0);
            QString min = time.split("_").at(1);
            QString sec = time.split("_").at(2);
            item->date_time.setTime(QTime(hour.toInt()
                                               ,min.toInt()
                                               ,sec.toInt()
                                               ));
            item->getLA_filetime()->setText(item->date_time.toString("yyyy:MM:dd hh:mm:ss"));
            qDebug()<<item->date_time.toString("yyyy:MM:dd hh:mm:ss");
            item->getLA_Matname()->setText(data_split.at(5));
            item_list.append(item);
        }
    }
    source_file.close();
    return true;
}

QStringList hanile_log_item::getSourceline_list() const
{
    return sourceline_list;
}

void hanile_log_item::setSourceline_list(const QStringList &value)
{
    sourceline_list = value;
}



QLabel *hanile_item::getLA_filetime() const
{
    return LA_filetime;
}

void hanile_item::setLA_filetime(QLabel *value)
{
    LA_filetime = value;
}

QLabel *hanile_item::getLA_layernumber() const
{
    return LA_layernumber;
}

void hanile_item::setLA_layernumber(QLabel *value)
{
    LA_layernumber = value;
}

hanile_item::hanile_item()
{
      LA_filename = new QLabel();
      LA_filetime = new QLabel();
      LA_layernumber = new QLabel();
      LA_Matname = new QLabel();
      LA_testcheck = new QLabel();
      data_start_point = 0;
}

QLabel *hanile_item::getLA_testcheck() const
{
    return LA_testcheck;
}

void hanile_item::setLA_testcheck(QLabel *value)
{
    LA_testcheck = value;
}

int hanile_item::getData_end_point() const
{
    return data_end_point;
}

void hanile_item::setData_end_point(int value)
{
    data_end_point = value;
}

QLabel *hanile_item::getLA_Matname() const
{
    return LA_Matname;
}

void hanile_item::setLA_Matname(QLabel *value)
{
    LA_Matname = value;
}


int hanile_item::getData_start_point() const
{
    return data_start_point;
}

void hanile_item::setData_start_point(int value)
{
    data_start_point = value;
}

QLabel *hanile_item::getLA_filename() const
{
    return LA_filename;
}

void hanile_item::setLA_filename(QLabel *value)
{
    LA_filename = value;
}
