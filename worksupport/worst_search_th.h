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
    int workerfail_deposition_count;
    double workerfail_deposition_rate;
    int workerfail_output_count;
    double workerfail_output_rate;

    int machinefail_probe_count;
    double machinefail_probe_rate;
    int machinefail_eatching_count;
    double machinefail_eatching_rate;
    int machinefail_light_count;
    double machinefail_light_rate;
    int machinefail_deposition_count;
    double machinefail_deposition_rate;

    int depositionpaticle_count;
    double depositionpaticle_rate;
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
    int jobmiss_deposition_count;
    double jobmiss_deposition_rate;
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
    int getWorkerfail_deposition_count() const;
    void setWorkerfail_deposition_count(int value);
    double getWorkerfail_deposition_rate() const;
    void setWorkerfail_deposition_rate(double value);
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
    int getMachinefail_deposition_count() const;
    void setMachinefail_deposition_count(int value);
    double getMachinefail_deposition_rate() const;
    void setMachinefail_deposition_rate(double value);
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
    int getDepositionpaticle_count() const;
    void setDepositionpaticle_count(int value);
    double getDepositionpaticle_rate() const;
    void setDepositionpaticle_rate(double value);
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
    int getJobmiss_deposition_count() const;
    void setJobmiss_deposition_count(int value);
    double getJobmiss_deposition_rate() const;
    void setJobmiss_deposition_rate(double value);
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
class rework_text_type{
public:
    QString product_name;
    QString LOT;
    QString process;
    int lot_count;
    int chip_count;
    QString content;


    QString getProduct_name() const;
    void setProduct_name(const QString &value);
    QString getLOT() const;
    void setLOT(const QString &value);
    QString getProcess() const;
    void setProcess(const QString &value);
    int getLot_count() const;
    void setLot_count(int value);
    int getChip_count() const;
    void setChip_count(int value);
    QString getContent() const;
    void setContent(const QString &value);
};

class defect_worst_type{
 public:
    QString product_name;
    QString LOT;
    QString process;
    int lot_count;
    int chip_count;
    QString Defect_name;
    QString content;
    QString metarial_grouptype;
    QString getProduct_name() const;
    void setProduct_name(const QString &value);
    QString getLOT() const;
    void setLOT(const QString &value);
    QString getProcess() const;
    void setProcess(const QString &value);
    int getLot_count() const;
    void setLot_count(int value);
    int getChip_count() const;
    void setChip_count(int value);
    QString getDefect_name() const;
    void setDefect_name(const QString &value);
    QString getContent() const;
    void setContent(const QString &value);

    QString getMetarial_grouptype() const;
    void setMetarial_grouptype(const QString &value);
};
class csp_limit_product_type{
    QString product_name;
    double input_sum;
    double output_sum;
    double vaild;

public:
    QString getProduct_name() const;
    void setProduct_name(const QString &value);
    double getInput_sum() const;
    void setInput_sum(double value);
    double getOutput_sum() const;
    void setOutput_sum(double value);
    double getVaild() const;
    void setVaild(double value);
};

class worst_search_th : public QThread
{
    Q_OBJECT
public:
    worst_search_th();
    QSqlDatabase ms_mesdb;
    QSqlDatabase my_mesdb;
    QVector<process_code> daily_worstcpsprocesslist;
    QVector<process_code> single_worstcpsprocesslist;
    QVector<process_code> single_accumulate_worstcpsprocesslist;
    QVector<process_code> DPX_worstcpsprocesslist;
    QVector<process_code> DPX_accumulate_worstcpsprocesslist;
    QVector<process_code> RSM_worstcpsprocesslist;
    QVector<process_code> RSM_accumulate_worstcpsprocesslist;
    QVector<process_code> accumulate_worstcpsprocesslist;
    QVector<rework_text_type> rework_worstcpsprocesslist;
    QVector<defect_worst_type> defect_worstcpsprocesslist;
    QVector<csp_limit_product_type> CSP_Limit_product_item_list;
    QStringList DEFECT_ITEM_LIST;
    QStringList EXTERIOR_DEFECT_ITEM_LIST;
    QStringList REWORK_ITEM_LIST;
    QStringList PROBE_ITEM_LIST;
    QStringList CSP_LIMIT_PRODUCT_LIST;
    QDate select_date;
    QStringList rework_content;
    double daily_totalvild;
    double single_daily_totalvild;
    double single_last_probevild;
    double single_accumulate_totalvild;
    double DPX_daily_totalvild;
    double DPX_last_probevild;
    double DPX_accumulate_totalvild;
    double RSM_daily_totalvild;
    double RSM_last_probevild;
    double RSM_accumulate_totalvild;

    double exterior_vaild;
    double exterior_worst_count;
    double accumulate_exterior_vaild;
    double accumulate_exterior_worst_count;

    double accumulate_totalvild;
    double last_probe_vild_daily;
    double accumulate_last_probe_vild_daily;
    double worst_sum;
    double accumulate_worst_sum;
    double total_DP001_worst;
    double total_DP003_worst;
    double total_DP004_worst;
    double total_DP005_worst;
    double total_DP006_worst;
    double total_DP008_worst;
    double accumulate_total_DP001_worst;
    double accumulate_total_DP003_worst;
    double accumulate_total_DP004_worst;
    double accumulate_total_DP005_worst;
    double accumulate_total_DP006_worst;
    double accumulate_total_DP008_worst;

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
    double total_sin_diseatching_worst;
    double total_brigit_pad_worst;

    double accumulate_total_Workerfail_probe;
    double accumulate_total_Workerfail_eatching;
    double accumulate_total_Workerfail_light;
    double accumulate_total_Workerfail_defect;
    double accumulate_total_Workerfail_output;
    double accumulate_total_Machinefail_probe;
    double accumulate_total_Machinefail_eatching;
    double accumulate_total_Machinefail_light;
    double accumulate_total_Machinefail_defect;
    double accumulate_total_Padpaticle;
    double accumulate_total_Etcpaticle;
    double accumulate_total_Pattenpaticle;
    double accumulate_total_Defectpaticle;
    double accumulate_total_sin_diseatching_worst;
    double accumulate_total_brigit_pad_worst;

    double total_Rework_paticle;
    double total_Jobmiss_defect;
    double total_Jobmiss_eatching;
    double total_Jobmiss_light;
    double total_Jobmiss_probe;
    double total_metalhigh;
    double total_metallow;
    double total_prcdhigh;
    double total_prcdlow;

    double accumulate_total_Rework_paticle;
    double accumulate_total_Jobmiss_defect;
    double accumulate_total_Jobmiss_eatching;
    double accumulate_total_Jobmiss_light;
    double accumulate_total_Jobmiss_probe;
    double accumulate_total_metalhigh;
    double accumulate_total_metallow;
    double accumulate_total_prcdhigh;
    double accumulate_total_prcdlow;

    double exterior_pattenpaticle_vaild;
    double exterior_padpaticle_vaild;
    double exterior_sindiseatching_vaild;
    double exterior_etcpaticle_vaild;
    double exterior_brigitpadworst_vaild;

    double accumulate_exterior_pattenpaticle_vaild;
    double accumulate_exterior_padpaticle_vaild;
    double accumulate_exterior_sindiseatching_vaild;
    double accumulate_exterior_etcpaticle_vaild;
    double accumulate_exterior_brigitpadworst_vaild;


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

    double getSingle_daily_totalvild() const;
    void setSingle_daily_totalvild(double value);

    double getSingle_accumulate_totalvild() const;
    void setSingle_accumulate_totalvild(double value);

    double getSingle_last_probevild() const;
    void setSingle_last_probevild(double value);

    double getDPX_daily_totalvild() const;
    void setDPX_daily_totalvild(double value);

    double getDPX_last_probevild() const;
    void setDPX_last_probevild(double value);

    double getDPX_accumulate_totalvild() const;
    void setDPX_accumulate_totalvild(double value);

    double getRSM_daily_totalvild() const;
    void setRSM_daily_totalvild(double value);

    double getRSM_last_probevild() const;
    void setRSM_last_probevild(double value);

    double getRSM_accumulate_totalvild() const;
    void setRSM_accumulate_totalvild(double value);

    QVector<rework_text_type> getRework_worstcpsprocesslist() const;
    void setRework_worstcpsprocesslist(const QVector<rework_text_type> &value);

    QVector<defect_worst_type> getDefect_worstcpsprocesslist() const;
    void setDefect_worstcpsprocesslist(const QVector<defect_worst_type> &value);

    double getTotal_sin_diseatching_worst() const;
    void setTotal_sin_diseatching_worst(double value);

    double getTotal_brigit_pad_worst() const;
    void setTotal_brigit_pad_worst(double value);

    double getExterior_vaild() const;
    void setExterior_vaild(double value);

    double getExterior_worst_count() const;
    void setExterior_worst_count(double value);


    double getExterior_pattenpaticle_vaild() const;
    void setExterior_pattenpaticle_vaild(double value);

    double getExterior_padpaticle_vaild() const;
    void setExterior_padpaticle_vaild(double value);

    double getExterior_sindiseatching_vaild() const;
    void setExterior_sindiseatching_vaild(double value);

    double getExterior_etcpaticle_vaild() const;
    void setExterior_etcpaticle_vaild(double value);

    double getExterior_brigitpadworst_vaild() const;
    void setExterior_brigitpadworst_vaild(double value);

    QVector<csp_limit_product_type> getCSP_Limit_product_item_list() const;
    void setCSP_Limit_product_item_list(const QVector<csp_limit_product_type> &value);

signals:
    void sig_excel_work();
    void sig_debug_output(QString str);

private:
    void run();
};

#endif // WORST_SEARCH_TH_H
