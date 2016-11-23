#include "nikon_log_err_chartview.h"

nikon_log_err_chartview::nikon_log_err_chartview(QChart *chart, QWidget *parent):
    QChartView(chart, parent),
    m_isTouching(false)
{
    setRubberBand(QChartView::RectangleRubberBand);

}

bool nikon_log_err_chartview::viewportEvent(QEvent *event)
{
//    if (event->type() == QEvent::TouchBegin) {
//        // By default touch events are converted to mouse events. So
//        // after this event we will get a mouse event also but we want
//        // to handle touch events as gestures only. So we need this safeguard
//        // to block mouse events that are actually generated from touch.
//        m_isTouching = true;

//        // Turn off animations when handling gestures they
//        // will only slow us down.
//        chart()->setAnimationOptions(QChart::NoAnimation);
//    }

    return QChartView::viewportEvent(event);
}

void nikon_log_err_chartview::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mousePressEvent(event);
}

void nikon_log_err_chartview::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
//    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void nikon_log_err_chartview::mouseMoveEvent(QMouseEvent *event)
{
//    QPointF value(chart()->mapToValue(event->pos()).x(),chart()->mapToValue(event->pos()).y());
//    emit move_value(value);
    if(chart()->objectName() == "nikon_err_bar_chart"){

    }else if(chart()->objectName() == "nikon_err_line_chart"){
        QPointF value(chart()->mapToValue(event->pos()).x(),chart()->mapToValue(event->pos()).y());
        emit move_value(value);
    }

    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void nikon_log_err_chartview::keyPressEvent(QKeyEvent *event)
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
