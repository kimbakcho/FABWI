#ifndef EIS_SELECT_COPYMODE_DIALOG_H
#define EIS_SELECT_COPYMODE_DIALOG_H

#include <QDialog>

namespace Ui {
class eis_select_copymode_dialog;
}

class eis_select_copymode_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit eis_select_copymode_dialog(QWidget *parent = 0);
    int mode;
    ~eis_select_copymode_dialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::eis_select_copymode_dialog *ui;
};

#endif // EIS_SELECT_COPYMODE_DIALOG_H
