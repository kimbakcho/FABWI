#ifndef TEMP_HUMI_WIDGET_H
#define TEMP_HUMI_WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QStandardItemModel>
#include <QSettings>
#include <QTcpSocket>
namespace Ui {
class temp_humi_widget;
}

class temp_humi_widget : public QWidget
{
    Q_OBJECT

public:
    explicit temp_humi_widget(QWidget *parent = 0);
    QTimer plantimer;
    QStandardItemModel *mainmodel;
    QString configini_str;
    QTcpSocket socket;
    QByteArray join_data;
    QByteArrayList datalist;
    QTimer temp_humi_timer;


    ~temp_humi_widget();

private slots:

    void host_connect();


    void model_change();

    void read_ready();

    void temp_humi_time_out();



private:
    Ui::temp_humi_widget *ui;
};

#endif // TEMP_HUMI_WIDGET_H
