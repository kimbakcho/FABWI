#ifndef ETCHING_ANALYSOR_H
#define ETCHING_ANALYSOR_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <etching_chart_1.h>
#include <etching_chart_1_view.h>
#include <QLineSeries>
#include <QColorDialog>
namespace Ui {
class etching_analysor;
}

class etching_analysor : public QWidget
{
    Q_OBJECT

public:
    explicit etching_analysor(QWidget *parent = 0);
    void analysor(QString filepath);
    QStringList dataline;
    QStringList data_type_list;
    int data_start_row;
    etching_chart_1 *chart;
    etching_chart_1_view *chartview;
    QLineSeries *line;
    QColor line_color;
    ~etching_analysor();
public slots:
    void chart_move_value(QPointF value);

private slots:
    void on_select_file_clicked();
    void read_ready_log();


    void on_cb_type_currentIndexChanged(int index);

    void on_cb_type_2_currentIndexChanged(int index);

    void on_zoomreset_btn_clicked();

    void on_line_add_btn_clicked();

    void on_select_color_btn_clicked();

private:
    Ui::etching_analysor *ui;
};

#endif // ETCHING_ANALYSOR_H
