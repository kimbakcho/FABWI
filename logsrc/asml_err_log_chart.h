#ifndef ASML_ERR_LOG_CHART_H
#define ASML_ERR_LOG_CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChart>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE


class ASML_err_log_chart : public QChart
{
    Q_OBJECT
public:
    ASML_err_log_chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
};

#endif // ASML_ERR_LOG_CHART_H
