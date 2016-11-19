#ifndef ETCHING_BASE_CHART_VIEW_H
#define ETCHING_BASE_CHART_VIEW_H

#include <QWidget>
#include <etching_chart_1.h>
#include <etching_chart_1_view.h>
#include <QColorDialog>
#include <etching_chart_bigview.h>
#include <QDebug>
namespace Ui {
class Etching_base_chart_view;
}

class Etching_base_chart_view : public QWidget
{
    Q_OBJECT

public:
    explicit Etching_base_chart_view(QWidget *parent = 0);
    void set_item_list_box(QStringList value);
    void set_type_list_box(QStringList value);
    QColor line_color;
    etching_chart_1 *mainchart;
    etching_chart_1_view *chartview;
    QStringList log_data;
    QStringList item_data;
    QStringList type_data;
    QLineSeries *main_line;
    int data_start_row;
    ~Etching_base_chart_view();

    QColor getLine_color() const;
    void setLine_color(const QColor &value);

    QStringList getLog_data() const;
    void setLog_data(const QStringList &value);

    void set_item_index(int index);
    void set_item_name(QString str);
    void set_type_index(int index);

    int getData_start_row() const;
    void setData_start_row(int value);
    Ui::Etching_base_chart_view *ui;
private slots:
    void on_cb_item_currentIndexChanged(int index);

    void on_cb_type_currentIndexChanged(int index);

    void on_select_color_btn_clicked();

    void on_zoomreset_btn_clicked();

    void chart_move_value(QPointF value);


    void on_view_linelabel_btn_clicked();

    void on_bigview_btn_clicked();

    void on_line_add_btn_clicked();

private:

};

#endif // ETCHING_BASE_CHART_VIEW_H
