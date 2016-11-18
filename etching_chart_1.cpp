#include "etching_chart_1.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>

etching_chart_1::etching_chart_1(QLineSeries *line,QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    setTitle("");

    addSeries(line);
    line->setPointsVisible(true);
    createDefaultAxes();
    setAcceptHoverEvents(true);



//    m_coordX = new QGraphicsSimpleTextItem(this);
//    m_coordX->setPos(size().width()/2 - 50, size().height());
//    m_coordX->setText("X: ");
//    m_coordY = new QGraphicsSimpleTextItem(this);
//    m_coordY->setPos(size().width()/2 + 50, size().height());
//    m_coordY->setText("Y: ");


}



