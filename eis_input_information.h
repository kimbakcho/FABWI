#ifndef EIS_INPUT_INFORMATION_H
#define EIS_INPUT_INFORMATION_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class EIS_input_information;
}

class EIS_input_information : public QDialog
{
    Q_OBJECT

public:
    explicit EIS_input_information(QWidget *parent = 0);
    QSqlDatabase db;
    void init_combo_box();
    ~EIS_input_information();

private slots:
    void on_add_team_btn_clicked();

    void on_add_process_btn_clicked();

    void on_add_facilities_btn_clicked();

    void on_add_people_btn_clicked();


private:
    Ui::EIS_input_information *ui;
};

#endif // EIS_INPUT_INFORMATION_H
