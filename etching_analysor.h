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
#include <etching_base_chart_view.h>
#include <QMimeData>
#include <etching_analysor_item.h>
#include <QHeaderView>
namespace Ui {
class etching_analysor;
}

class etching_analysor : public QWidget
{
    Q_OBJECT

public:
    explicit etching_analysor(QWidget *parent = 0);
    void analysor(etching_analysor_item *item);
    int file_drag_count;
    etching_chart_1 *chart;
    etching_chart_1_view *chartview;
    QLineSeries *line;
    QColor line_color;
    QVector<etching_analysor_item *>logfile;
    QVector<Etching_base_chart_view *> base_chartview_list;
    ~etching_analysor();
public slots:


private slots:
//    void on_select_file_clicked();

    void on_chart_listview_cellDoubleClicked(int row, int column);

    void change_line_show(bool ischeck,QColor linecolor,QString waferid);

private:
    Ui::etching_analysor *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // ETCHING_ANALYSOR_H
