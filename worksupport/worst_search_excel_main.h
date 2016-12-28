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
#include <worksupport/worst_search_th.h>
#include <QFileDialog>
#include <worksupport/worst_search_wlp_th.h>
#include <QSqlTableModel>
#include <QTimer>

namespace Ui {
class worst_search_excel_main;
}

class worst_search_excel_main : public QWidget
{
    Q_OBJECT

public:
    explicit worst_search_excel_main(QWidget *parent = 0);
    QSqlDatabase my_mesdb;
    QSqlDatabase litedb;
    QSqlTableModel *CSP_model;
    QSqlTableModel *WLP_model;
    QSqlTableModel *work_plan_model;
    QSqlTableModel *type_price_model;
    QTimer work_plan_timer;



    ~worst_search_excel_main();

private slots:
    void on_start_btn_clicked();
    void closeEvent(QCloseEvent *event);
    void slot_excel_work();
    void slot_excel_WLP_work();

    void on_fileselect_dialog_btn_clicked();
    void slot_debug_output(QString str);
    void slot_debug_output_WLP(QString str);

    void on_fileselect_WLP_dialog_btn_clicked();

    void on_CSP_Process_add_btn_clicked();

    void on_CSP_Process_del_btn_clicked();

    void on_WLP_Process_add_btn_clicked();

    void on_WLP_Process_del_btn_clicked();

    void on_work_plan_add_btn_clicked();

    void work_plan_timer_timeout();

    void on_work_plan_del_btn_clicked();


    void on_type_price_add_btn_clicked();

    void on_type_price_del_btn_clicked();

private:
    Ui::worst_search_excel_main *ui;
};

#endif // WORST_SEARCH_EXCEL_MAIN_H
