#ifndef HANILE_LOG_ITEM_H
#define HANILE_LOG_ITEM_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QDateTime>
class hanile_item{


public:
    hanile_item();
    QLabel *LA_filename;
    QLabel *LA_filetime;
    QLabel *LA_layernumber;
    QLabel *LA_Matname;
    int data_start_point;
    int data_end_point;
    QDateTime date_time;
    QLabel *LA_testcheck;

    QLabel *getLA_filename() const;
    void setLA_filename(QLabel *value);
    QLabel *getLA_filetime() const;
    void setLA_filetime(QLabel *value);
    QLabel *getLA_layernumber() const;
    void setLA_layernumber(QLabel *value);
    int getData_start_point() const;
    void setData_start_point(int value);
    QLabel *getLA_Matname() const;
    void setLA_Matname(QLabel *value);
    int getData_end_point() const;
    void setData_end_point(int value);
    QLabel *getLA_testcheck() const;
    void setLA_testcheck(QLabel *value);
};

namespace Ui {
class hanile_log_item;
}

class hanile_log_item : public QWidget
{
    Q_OBJECT

public:
    explicit hanile_log_item(QWidget *parent = 0);
    QString file_path;
    QFile source_file;

    ~hanile_log_item();

    bool file_read();
    QStringList sourceline_list;

    QVector<hanile_item *> item_list;

    QString getFile_path() const;
    void setFile_path(const QString &value);

    QStringList getSourceline_list() const;
    void setSourceline_list(const QStringList &value);



private:

    Ui::hanile_log_item *ui;
};

#endif // HANILE_LOG_ITEM_H
