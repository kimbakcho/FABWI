#ifndef HANILE_LOG_MAIN_H
#define HANILE_LOG_MAIN_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <Qdir>
#include <QFile>
#include <hanile_log_item.h>
#include <QtCharts/QChart>
#include <QLineSeries>
#include <QXYSeries>
#include <hanile_log_chartview.h>
#include <hanile_log_err_chart.h>
#include <QDateTimeAxis>
#include <QValueAxis>
#define RATE 3
#define POWER 4
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class hanile_log_main;
}

class hanile_log_main : public QWidget
{
    Q_OBJECT

public:
    explicit hanile_log_main(QWidget *parent = 0);
    QVector<hanile_log_item *> item_list;
    hanile_log_err_chart *main_chart;
    hanile_log_chartview *main_chartview;
    QDateTimeAxis  *axisX ;
    QValueAxis  *axisY ;
    ~hanile_log_main();

private slots:
    void on_MCB_power_toggled(bool checked);

    void on_MZ_reset_btn_clicked();

    void on_MCB_rate_toggled(bool checked);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    Ui::hanile_log_main *ui;
};

#endif // HANILE_LOG_MAIN_H
