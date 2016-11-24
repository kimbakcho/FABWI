#ifndef NIKON_LOG_ERR_MAIN_H
#define NIKON_LOG_ERR_MAIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QDateTime>
#include <QDir>
#include <QProgressDialog>
#include <logsrc/nikon_log_err_thread.h>
#include <logsrc/nikon_log_err_chart.h>
#include <logsrc/nikon_log_err_chartview.h>
#include <QBarset>
#include <logsrc/nikon_log_err_list_item.h>

#include <QtCharts/QChart>
#include <QLineSeries>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QLabel>
QT_CHARTS_USE_NAMESPACE


namespace Ui {
class nikon_log_err_main;
}

class data_infrom{
public :

    QString data;
    int count = 0;
    data_infrom(QString data,int count){
        this->data = data;
        this->count = count;
    }
};

class nikon_log_err_main : public QWidget
{
    Q_OBJECT

public:
    explicit nikon_log_err_main(QWidget *parent = 0);
    QStringList sourceline_list;
    QSqlDatabase db;
    QStringList file_names;
    QString current_table_name;
    QString current_count_table_name;
    nikon_log_err_chart *bar_chart;
    nikon_log_err_chart *time_chart;
    nikon_log_err_chartview *bar_chart_view;
    nikon_log_err_chartview *time_chart_view;
    QVector<nikon_log_err_list_item *> item_list;
    QLineSeries *lineseries;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    void analysor(QStringList dataline);
    void analysor2();
    QBarSet *barset;
    QBarSeries *series;
    QVector<data_infrom *> datalist;

    ~nikon_log_err_main();
private slots:
    void slot_barset_hover_change(QString str);
    void on_bar_chart_zoomreset_btn_clicked();
    void slot_toggle(bool data);

    void on_bar_list_view_cellClicked(int row, int column);

    void on_line_chart_zoomreset_btn_clicked();

    void on_show_line_data_btn_clicked();

    void slot_move_point(QPointF point);

private:
    Ui::nikon_log_err_main *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};


#endif // NIKON_LOG_ERR_MAIN_H
