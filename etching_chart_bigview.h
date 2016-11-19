#ifndef ETCHING_CHART_BIGVIEW_H
#define ETCHING_CHART_BIGVIEW_H

#include <QWidget>
#include <QGridLayout>

namespace Ui {
class etching_chart_bigview;
}

class etching_chart_bigview : public QWidget
{
    Q_OBJECT

public:
    explicit etching_chart_bigview(QObject *parent_view);
    Ui::etching_chart_bigview *ui;
    QObject *parent_view;

    ~etching_chart_bigview();



private:
    void closeEvent(QCloseEvent *event);

};

#endif // ETCHING_CHART_BIGVIEW_H
