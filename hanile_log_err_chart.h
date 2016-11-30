#ifndef HANILE_LOG_ERR_CHART_H
#define HANILE_LOG_ERR_CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE


class hanile_log_err_chart : public QChart
{
public:
    hanile_log_err_chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
};

#endif // HANILE_LOG_ERR_CHART_H
