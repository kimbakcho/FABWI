#ifndef EIS_SERARCH_ITEM_H
#define EIS_SERARCH_ITEM_H

#include <QWidget>

namespace Ui {
class EIS_serarch_item;
}

class EIS_serarch_item : public QWidget
{
    Q_OBJECT

public:
    explicit EIS_serarch_item(QWidget *parent = 0);
    Ui::EIS_serarch_item *ui;
    ~EIS_serarch_item();

private:

};

#endif // EIS_SERARCH_ITEM_H
