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


private slots:
//    void on_select_file_clicked();

private:
    Ui::etching_analysor *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // ETCHING_ANALYSOR_H
