#ifndef EIS_ALARM_SERARCH_ITEM_H
#define EIS_ALARM_SERARCH_ITEM_H

#include <QWidget>


namespace Ui {
class eis_alarm_serarch_item;
}

class eis_alarm_serarch_item : public QWidget
{
    Q_OBJECT

public:
    explicit eis_alarm_serarch_item(QWidget *parent = 0);


    ~eis_alarm_serarch_item();

    Ui::eis_alarm_serarch_item *ui;

private:

};

#endif // EIS_ALARM_SERARCH_ITEM_H
