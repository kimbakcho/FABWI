#ifndef EIS_DELETE_DIALOG_H
#define EIS_DELETE_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class EIS_delete_dialog;
}

class EIS_delete_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit EIS_delete_dialog(QWidget *parent = 0);
    QSqlDatabase db;
    bool password_alright;
    ~EIS_delete_dialog();

    bool getPassword_alright() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EIS_delete_dialog *ui;
};

#endif // EIS_DELETE_DIALOG_H
