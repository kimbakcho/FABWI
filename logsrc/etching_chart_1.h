#ifndef ETCHING_CHART_1_H
#define ETCHING_CHART_1_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE


class etching_chart_1 : public QChart
{
    Q_OBJECT
public:
    etching_chart_1(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    QVector<QLineSeries *> *linedata_list;


private:


};

#endif // ETCHING_CHART_1_H
