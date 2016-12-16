#ifndef WORST_SEARCH_EXCEL_MAIN_H
#define WORST_SEARCH_EXCEL_MAIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <global_define.h>
#include <QMessageBox>
#include <QVector>
#include <worst_search_th.h>
#include <QFileDialog>

namespace Ui {
class worst_search_excel_main;
}

class worst_search_excel_main : public QWidget
{
    Q_OBJECT

public:
    explicit worst_search_excel_main(QWidget *parent = 0);



    ~worst_search_excel_main();

private slots:
    void on_start_btn_clicked();
    void closeEvent(QCloseEvent *event);
    void slot_excel_work();

    void on_fileselect_dialog_btn_clicked();

private:
    Ui::worst_search_excel_main *ui;
};

#endif // WORST_SEARCH_EXCEL_MAIN_H
