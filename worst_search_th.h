#ifndef WORST_SEARCH_TH_H
#define WORST_SEARCH_TH_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <global_define.h>
#include <QMessageBox>
#include <QVector>
#include <QDateTime>
#include <QApplication>
#include <QFile>
#include <QAxObject>
#define REAL_QUERY
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

    int prcdhigh_count;
    double prcdhigh_rate;
    int prcdlow_count;
    double prcdlow_rate;

    int metalhigh_count;
    double metalhigh_rate;
    int metallow_count;
    double metallow_rate;



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
    int getPrcdhigh_count() const;
    void setPrcdhigh_count(int value);
    double getPrcdhigh_rate() const;
    void setPrcdhigh_rate(double value);
    int getPrcdlow_count() const;
    void setPrcdlow_count(int value);
    double getPrcdlow_rate() const;
    void setPrcdlow_rate(double value);
    int getMetalhigh_count() const;
    void setMetalhigh_count(int value);
    double getMetalhigh_rate() const;
    void setMetalhigh_rate(double value);
    int getMetallow_count() const;
    void setMetallow_count(int value);
    double getMetallow_rate() const;
    void setMetallow_rate(double value);
};

class worst_search_th : public QThread
{
    Q_OBJECT
public:
    worst_search_th();
    QSqlDatabase ms_mesdb;
    QSqlDatabase my_mesdb;
    QVector<process_code> daily_worstcpsprocesslist;
    QVector<process_code> accumulate_worstcpsprocesslist;
    QStringList DEFECT_ITEM_LIST;
    QStringList REWORK_ITEM_LIST;
    QStringList PROBE_ITEM_LIST;
    QDate select_date;
    double daily_totalvild;
    double accumulate_totalvild;
    double last_probe_vild_daily;
    double worst_sum;
    double total_DP001_worst;
    double total_DP003_worst;
    double total_DP004_worst;
    double total_DP005_worst;
    double total_DP006_worst;
    double total_DP008_worst;

    double total_Workerfail_probe;
    double total_Workerfail_eatching;
    double total_Workerfail_light;
    double total_Workerfail_defect;
    double total_Workerfail_output;
    double total_Machinefail_probe;
    double total_Machinefail_eatching;
    double total_Machinefail_light;
    double total_Machinefail_defect;
    double total_Padpaticle;
    double total_Etcpaticle;
    double total_Pattenpaticle;
    double total_Defectpaticle;

    double total_Rework_paticle;
    double total_Jobmiss_defect;
    double total_Jobmiss_eatching;
    double total_Jobmiss_light;
    double total_Jobmiss_probe;
    double total_metalhigh;
    double total_metallow;
    double total_prcdhigh;
    double total_prcdlow;


    QVector<product_item *> item_vector;
    QDate getSelect_date() const;
    void setSelect_date(const QDate &value);
    double getDaily_totalvild() const;
    void setDaily_totalvild(double value);

    double getAccumulate_totalvild() const;
    void setAccumulate_totalvild(double value);

    double getWorst_sum() const;
    void setWorst_sum(double value);

    double getTotal_DP001_worst() const;
    void setTotal_DP001_worst(double value);

    double getTotal_DP003_worst() const;
    void setTotal_DP003_worst(double value);

    double getTotal_DP004_worst() const;
    void setTotal_DP004_worst(double value);

    double getTotal_DP005_worst() const;
    void setTotal_DP005_worst(double value);

    double getTotal_DP006_worst() const;
    void setTotal_DP006_worst(double value);

    double getTotal_DP008_worst() const;
    void setTotal_DP008_worst(double value);

    double getTotal_Workerfail_probe() const;
    void setTotal_Workerfail_probe(double value);

    double getTotal_Workerfail_eatching() const;
    void setTotal_Workerfail_eatching(double value);

    double getTotal_Workerfail_light() const;
    void setTotal_Workerfail_light(double value);

    double getTotal_Workerfail_defect() const;
    void setTotal_Workerfail_defect(double value);

    double getTotal_Workerfail_output() const;
    void setTotal_Workerfail_output(double value);

    double getTotal_Machinefail_probe() const;
    void setTotal_Machinefail_probe(double value);

    double getTotal_Machinefail_eatching() const;
    void setTotal_Machinefail_eatching(double value);

    double getTotal_Machinefail_light() const;
    void setTotal_Machinefail_light(double value);

    double getTotal_Machinefail_defect() const;
    void setTotal_Machinefail_defect(double value);

    double getTotal_Padpaticle() const;
    void setTotal_Padpaticle(double value);

    double getTotal_Etcpaticle() const;
    void setTotal_Etcpaticle(double value);

    double getTotal_Pattenpaticle() const;
    void setTotal_Pattenpaticle(double value);

    double getTotal_Defectpaticle() const;
    void setTotal_Defectpaticle(double value);

    double getTotal_Rework_paticle() const;
    void setTotal_Rework_paticle(double value);

    double getTotal_Jobmiss_defect() const;
    void setTotal_Jobmiss_defect(double value);

    double getTotal_Jobmiss_eatching() const;
    void setTotal_Jobmiss_eatching(double value);

    double getTotal_Jobmiss_light() const;
    void setTotal_Jobmiss_light(double value);

    double getTotal_Jobmiss_probe() const;
    void setTotal_Jobmiss_probe(double value);

    double getTotal_metalhigh() const;
    void setTotal_metalhigh(double value);

    double getTotal_metallow() const;
    void setTotal_metallow(double value);

    double getTotal_prcdhigh() const;
    void setTotal_prcdhigh(double value);

    double getTotal_prcdlow() const;
    void setTotal_prcdlow(double value);

    double getLast_probe_vild_daily() const;
    void setLast_probe_vild_daily(double value);

signals:
    void sig_excel_work();

private:
    void run();
};

#endif // WORST_SEARCH_TH_H
