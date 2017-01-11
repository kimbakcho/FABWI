#include "hanile_log_chartview.h"

hanile_log_chartview::hanile_log_chartview(QChart *chart, QWidget *parent):
    QChartView(chart, parent),
    m_isTouching(false)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool hanile_log_chartview::viewportEvent(QEvent *event)
{
    return QChartView::viewportEvent(event);
}

void hanile_log_chartview::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mousePressEvent(event);
}

void hanile_log_chartview::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
//    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void hanile_log_chartview::mouseMoveEvent(QMouseEvent *event)
{

        QPointF value(chart()->mapToValue(event->pos()).x(),chart()->mapToValue(event->pos()).y());
        emit move_value(value);

    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void hanile_log_chartview::keyPressEvent(QKeyEvent *event)
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
