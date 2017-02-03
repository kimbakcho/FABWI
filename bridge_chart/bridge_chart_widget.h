#ifndef BRIDGE_CHART_WIDGET_H
#define BRIDGE_CHART_WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QSqlError>
#include <bridge_chart/bridge_chart.h>
#include <bridge_chart/bridge_chartview.h>
#include <QSqlQueryModel>
namespace Ui {
class bridge_chart_widget;
}

class bridge_chart_widget : public QWidget
{
    Q_OBJECT

public:
    explicit bridge_chart_widget(QWidget *parent = 0);
    bridge_chart *main_chart;
    bridge_chartview *main_chartview;
    QSqlDatabase my_db;
    QSqlDatabase ms_mes_db;
    QLineSeries *value_series;
    QLineSeries *CL_series;
    QLineSeries *USL_series[3];
    QLineSeries *LSL_series[3];
    QDateTimeAxis *axisX ;
    QValueAxis *axisY;
    QSqlQueryModel table_model;
    ~bridge_chart_widget();

private slots:
    void on_search_btn_clicked();

    void on_zoom_reset_btn_clicked();

    void on_point_visiable_btn_clicked();

private:
    void closeEvent(QCloseEvent *event);
    Ui::bridge_chart_widget *ui;
};

#endif // BRIDGE_CHART_WIDGET_H
