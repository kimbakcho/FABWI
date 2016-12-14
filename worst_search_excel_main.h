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
#define RoundOff(x, dig) (floor((x) * pow(10,dig) + 0.5) / pow(10,dig))
class process_code{
public:
    process_code();
    QString id;
    QString name;
    double INPUT_SUM;
    double OUTPUT_SUM;
    double DEFECT_SUM;
    double EXCLUDE_YIELD_QTY_SUM;
    double vield;
    QString getId() const;
    void setId(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    double getINPUT_SUM() const;
    void setINPUT_SUM(double value);
    double getOUTPUT_SUM() const;
    void setOUTPUT_SUM(double value);
    double getVield() const;
    void setVield(double value);
    double getDEFECT_SUM() const;
    void setDEFECT_SUM(double value);
    double getEXCLUDE_YIELD_QTY_SUM() const;
    void setEXCLUDE_YIELD_QTY_SUM(double value);
};

class product_item{
public:
    product_item(QString product_name);
    QString product_name;
    int total_fail_count;
    int probe_DP001_count;
    double probe_DP001_rate;
    int probe_DP002_count;
    double probe_DP002_rate;
    int probe_DP003_count;
    double probe_DP003_rate;
    int probe_DP004_count;
    double probe_DP004_rate;
    int probe_DP005_count;
    double probe_DP005_rate;
    int probe_DP006_count;
    double probe_DP006_rate;
    int probe_DP008_count;
    double probe_DP008_rate;

    int workerfail_probe_count;
    double workerfail_probe_rate;
    int workerfail_eatching_count;
    double workerfail_eatching_rate;
    int workerfail_light_count;
    double workerfail_light_rate;
    int workerfail_defect_count;
    double workerfail_defect_rate;
    int workerfail_output_count;
    double workerfail_output_rate;

    int machinefail_probe_count;
    double machinefail_probe_rate;
    int machinefail_eatching_count;
    double machinefail_eatching_rate;
    int machinefail_light_count;
    double machinefail_light_rate;
    int machinefail_defect_count;
    double machinefail_defect_rate;

    int defectpaticle_count;
    double defectpaticle_rate;
    int padpaticle_count;
    double padpaticle_rate;
    int etcpaticle_count;
    double etcpaticle_rate;
    int pattenpaticle_count;
    double pattenpaticle_rate;

    int jobmiss_probe_count;
    double jobmiss_probe_rate;
    int jobmiss_eatching_count;
    double jobmiss_eatching_rate;
    int jobmiss_light_count;
    double jobmiss_light_rate;
    int jobmiss_defect_count;
    double jobmiss_defect_rate;
    int rework_paticle_count;
    double rework_paticle_rate;


    int getTotal_fail_count() const;
    void setTotal_fail_count(int value);
    QString getProduct_name() const;
    void setProduct_name(const QString &value);
    int getProbe_DP001_count() const;
    void setProbe_DP001_count(int value);
    double getProbe_DP001_rate() const;
    void setProbe_DP001_rate(double value);
    int getProbe_DP002_count() const;
    void setProbe_DP002_count(int value);
    double getProbe_DP002_rate() const;
    void setProbe_DP002_rate(double value);
    int getProbe_DP003_count() const;
    void setProbe_DP003_count(int value);
    double getProbe_DP003_rate() const;
    void setProbe_DP003_rate(double value);
    int getProbe_DP004_count() const;
    void setProbe_DP004_count(int value);
    double getProbe_DP004_rate() const;
    void setProbe_DP004_rate(double value);
    int getProbe_DP006_count() const;
    void setProbe_DP006_count(int value);
    double getProbe_DP006_rate() const;
    void setProbe_DP006_rate(double value);
    int getProbe_DP008_count() const;
    void setProbe_DP008_count(int value);
    double getProbe_DP008_rate() const;
    void setProbe_DP008_rate(double value);
    int getProbe_DP005_count() const;
    void setProbe_DP005_count(int value);
    double getProbe_DP005_rate() const;
    void setProbe_DP005_rate(double value);
    int getWorkerfail_probe_count() const;
    void setWorkerfail_probe_count(int value);
    double getWorkerfail_probe_rate() const;
    void setWorkerfail_probe_rate(double value);
    int getWorkerfail_eatching_count() const;
    void setWorkerfail_eatching_count(int value);
    double getWorkerfail_eatching_rate() const;
    void setWorkerfail_eatching_rate(double value);
    int getWorkerfail_light_count() const;
    void setWorkerfail_light_count(int value);
    double getWorkerfail_light_rate() const;
    void setWorkerfail_light_rate(double value);
    int getWorkerfail_defect_count() const;
    void setWorkerfail_defect_count(int value);
    double getWorkerfail_defect_rate() const;
    void setWorkerfail_defect_rate(double value);
    int getWorkerfail_output_count() const;
    void setWorkerfail_output_count(int value);
    double getWorkerfail_output_rate() const;
    void setWorkerfail_output_rate(double value);
    int getMachinefail_probe_count() const;
    void setMachinefail_probe_count(int value);
    double getMachinefail_probe_rate() const;
    void setMachinefail_probe_rate(double value);
    int getMachinefail_eatching_count() const;
    void setMachinefail_eatching_count(int value);
    double getMachinefail_eatching_rate() const;
    void setMachinefail_eatching_rate(double value);
    int getMachinefail_light_count() const;
    void setMachinefail_light_count(int value);
    double getMachinefail_light_rate() const;
    void setMachinefail_light_rate(double value);
    int getMachinefail_defect_count() const;
    void setMachinefail_defect_count(int value);
    double getMachinefail_defect_rate() const;
    void setMachinefail_defect_rate(double value);
    int getPadpaticle_count() const;
    void setPadpaticle_count(int value);
    double getPadpaticle_rate() const;
    void setPadpaticle_rate(double value);
    int getEtcpaticle_count() const;
    void setEtcpaticle_count(int value);
    double getEtcpaticle_rate() const;
    void setEtcpaticle_rate(double value);
    int getPattenpaticle_count() const;
    void setPattenpaticle_count(int value);
    double getPattenpaticle_rate() const;
    void setPattenpaticle_rate(double value);
    int getDefectpaticle_count() const;
    void setDefectpaticle_count(int value);
    double getDefectpaticle_rate() const;
    void setDefectpaticle_rate(double value);
    int getRework_paticle_count() const;
    void setRework_paticle_count(int value);
    double getRework_paticle_rate() const;
    void setRework_paticle_rate(double value);
    int getJobmiss_probe_count() const;
    void setJobmiss_probe_count(int value);
    double getJobmiss_probe_rate() const;
    void setJobmiss_probe_rate(double value);
    int getJobmiss_eatching_count() const;
    void setJobmiss_eatching_count(int value);
    double getJobmiss_eatching_rate() const;
    void setJobmiss_eatching_rate(double value);
    int getJobmiss_light_count() const;
    void setJobmiss_light_count(int value);
    double getJobmiss_light_rate() const;
    void setJobmiss_light_rate(double value);
    int getJobmiss_defect_count() const;
    void setJobmiss_defect_count(int value);
    double getJobmiss_defect_rate() const;
    void setJobmiss_defect_rate(double value);
};

namespace Ui {
class worst_search_excel_main;
}

class worst_search_excel_main : public QWidget
{
    Q_OBJECT

public:
    explicit worst_search_excel_main(QWidget *parent = 0);
    QSqlDatabase ms_mesdb;
    QSqlDatabase my_mesdb;
    QVector<process_code> daily_worstcpsprocesslist;
    QVector<process_code> accumulate_worstcpsprocesslist;
    QStringList DEFECT_ITEM_LIST;
    QStringList REWORK_ITEM_LIST;
    QStringList PROBE_ITEM_LIST;

    QVector<product_item *> item_vector;
    ~worst_search_excel_main();

private slots:
    void on_start_btn_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::worst_search_excel_main *ui;
};

#endif // WORST_SEARCH_EXCEL_MAIN_H
