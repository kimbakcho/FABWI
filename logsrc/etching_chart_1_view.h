#ifndef ETCHING_CHART_1_VIEW_H
#define ETCHING_CHART_1_VIEW_H

#include <QObject>
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QDebug>
#include <QPointF>
QT_CHARTS_USE_NAMESPACE

class etching_chart_1_view : public QChartView
{
    Q_OBJECT
public:
    etching_chart_1_view(QChart *chart, QWidget *parent = 0);
signals:
    void move_value(QPointF value);

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    bool m_isTouching;
};

#endif // ETCHING_CHART_1_VIEW_H
