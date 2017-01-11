#ifndef OPERATING_RATIO_MAIN_H
#define OPERATING_RATIO_MAIN_H

#include <QWidget>
#include <operatingratio/operating_ratio_action.h>

namespace Ui {
class operating_ratio_main;
}

class operating_ratio_main : public QWidget
{
    Q_OBJECT

public:
    explicit operating_ratio_main(QWidget *parent = 0);
    ~operating_ratio_main();

private slots:
    void on_action_btn_clicked();

private:
    Ui::operating_ratio_main *ui;
};

#endif // OPERATING_RATIO_MAIN_H
