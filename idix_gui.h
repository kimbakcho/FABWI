#ifndef IDIX_GUI_H
#define IDIX_GUI_H

#include <QWidget>

namespace Ui {
class IDIX_gui;
}

class IDIX_gui : public QWidget
{
    Q_OBJECT

public:
    explicit IDIX_gui(QWidget *parent = 0);
    ~IDIX_gui();

private:
    Ui::IDIX_gui *ui;
};

#endif // IDIX_GUI_H
