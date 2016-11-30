#ifndef ASML_ERR_LOG_CHARTVIEW_H
#define ASML_ERR_LOG_CHARTVIEW_H

#include <QObject>
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QDebug>
#include <QPointF>
QT_CHARTS_USE_NAMESPACE


class ASML_err_log_chartview : public QChartView
{
     Q_OBJECT
public:
    ASML_err_log_chartview(QChart *chart, QWidget *parent = 0);
signals :
    void move_value(QPointF value);
protected :
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    bool m_isTouching;
};

#endif // ASML_ERR_LOG_CHARTVIEW_H
