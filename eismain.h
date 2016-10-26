#ifndef EISMAIN_H
#define EISMAIN_H

#include <QWidget>

namespace Ui {
class EISmain;
}

class EISmain : public QWidget
{
    Q_OBJECT

public:
    explicit EISmain(QWidget *parent = 0);
    ~EISmain();

private:
    Ui::EISmain *ui;
};

#endif // EISMAIN_H
