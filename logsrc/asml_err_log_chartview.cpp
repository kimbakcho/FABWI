#include "asml_err_log_chartview.h"

ASML_err_log_chartview::ASML_err_log_chartview(QChart *chart, QWidget *parent):
    QChartView(chart, parent),
    m_isTouching(false)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool ASML_err_log_chartview::viewportEvent(QEvent *event)
{
     return QChartView::viewportEvent(event);
}

void ASML_err_log_chartview::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;

    QChartView::mousePressEvent(event);
}

void ASML_err_log_chartview::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
//    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void ASML_err_log_chartview::mouseMoveEvent(QMouseEvent *event)
{
    if(chart()->objectName() == "asml_err_bar_cahrt"){

    }else if(chart()->objectName() == "asml_err_line_chart"){
        QPointF value(chart()->mapToValue(event->pos()).x(),chart()->mapToValue(event->pos()).y());
        emit move_value(value);
    }

    if (m_isTouching)
        return;

    QChartView::mouseMoveEvent(event);
}

void ASML_err_log_chartview::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
