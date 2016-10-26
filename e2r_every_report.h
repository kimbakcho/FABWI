#ifndef E2R_EVERY_REPORT_H
#define E2R_EVERY_REPORT_H

#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QProcess>
#include <QTextCodec>
#include <qstring.h>
#include <qtextcodec.h>
#include <qstring.h>
#include <QMessageBox>



namespace Ui {
class E2R_every_report;
}

class E2R_every_report : public QWidget
{
    Q_OBJECT

public:
    explicit E2R_every_report(QWidget *parent = 0);
    QString configini_str;
    QString toUniString(QString str);
    ~E2R_every_report();

private slots:
    void on_ICP_Etch_rate_btn_clicked();

    void on_export_btn_clicked();

    void on_export_path_btn_clicked();

    void on_excel_mecro_clicked();

    void on_SIN_Etch_rate_btn_clicked();

    void on_SIO2_Etch_rate_btn_clicked();

    void on_WPL_SiO2_btn_clicked();

    void on_ICP_Ashing_rate_btn_clicked();

    void on_mesure_1_btn_clicked();

    void on_mesure_2_3_btn_clicked();

    void on_sungmak_day_btn_clicked();

    void on_uniformity_ckeck_btn_clicked();

    void on_daily_gap_btn_clicked();

    void on_Daily_Bridge_btn_clicked();

    void on_Daily_pad_btn_clicked();

    void on_daily_pad_ashing_btn_clicked();

    void on_mesure_pr_cd_btn_clicked();

private:
    Ui::E2R_every_report *ui;
};

#endif // E2R_EVERY_REPORT_H
