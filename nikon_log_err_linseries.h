#ifndef NIKON_LOG_ERR_LINSERIES_H
#define NIKON_LOG_ERR_LINSERIES_H

#include <QObject>
#include <QtCharts/QChart>
#include <QLineSeries>
#include <QXYSeries>
QT_CHARTS_USE_NAMESPACE

class nikon_log_err_linseries : public QLineSeries
{
public:
    nikon_log_err_linseries();
};

#endif // NIKON_LOG_ERR_LINSERIES_H
