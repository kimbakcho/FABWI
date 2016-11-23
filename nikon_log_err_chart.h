#ifndef NIKO_LOG_ERR_CHART_H
#define NIKO_LOG_ERR_CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE

class nikon_log_err_chart : public QChart
{
     Q_OBJECT
public:
    nikon_log_err_chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
};

#endif // NIKO_LOG_ERR_CHART_H
