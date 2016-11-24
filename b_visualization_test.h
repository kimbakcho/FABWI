#ifndef B_VISUALIZATION_TEST_H
#define B_VISUALIZATION_TEST_H

#include <QWidget>
#include <QtDataVisualization>

using namespace QtDataVisualization;

namespace Ui {
class b_visualization_test;
}

class b_visualization_test : public QWidget
{
    Q_OBJECT

public:
    explicit b_visualization_test(QWidget *parent = 0);
    ~b_visualization_test();

private:
    Ui::b_visualization_test *ui;
};

#endif // B_VISUALIZATION_TEST_H
