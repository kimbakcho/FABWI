#ifndef ASML_ERR_LOG_MAIN_H
#define ASML_ERR_LOG_MAIN_H

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
#include <logsrc/asml_err_log_chart.h>
#include <logsrc/asml_err_log_chartview.h>
#include <QBarset>
#include <logsrc/asml_err_log_list_item.h>

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
class ASML_err_log_main;
}

class data_infrom_1{
public :

    QString data;
    int count = 0;
    QString error;
    data_infrom_1(QString data,int count,QString error){
        this->data = data;
        this->count = count;
        this->error = error;
    }
};

class ASML_err_log_main : public QWidget
{
    Q_OBJECT

public:
    explicit ASML_err_log_main(QWidget *parent = 0);
    QStringList sourceline_list;
    QSqlDatabase db;
    QStringList file_names;
    QString current_table_name;
    QString current_count_table_name;
    ASML_err_log_chart *bar_chart;
    QChart *time_chart;
    ASML_err_log_chartview *bar_chart_view;
    ASML_err_log_chartview *time_chart_view;
    QVector<ASML_err_log_list_item *> item_list;
    QLineSeries *lineseries;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    void analysor(QStringList dataline);
    void analysor2();
    QBarSet *barset;
    QBarSeries *series;
    QVector<data_infrom_1 *> datalist;
    ~ASML_err_log_main();
private slots:
    void slot_barset_hover_change(QString str, QString error);
    void on_bar_chart_zoomreset_btn_clicked();
    void slot_toggle(bool data);

    void on_bar_list_view_cellClicked(int row, int column);

    void on_line_chart_zoomreset_btn_clicked();

    void on_show_line_data_btn_clicked();

    void slot_move_point(QPointF point);
private:
    Ui::ASML_err_log_main *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // ASML_ERR_LOG_MAIN_H
