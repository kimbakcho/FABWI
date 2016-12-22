#ifndef WORST_SEARCH_WLP_TH_H
#define WORST_SEARCH_WLP_TH_H

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
#define REAL_QUERY_WLP
#define RoundOff(x, dig) (floor((x) * pow(10,dig) + 0.5) / pow(10,dig))
class process_code_WLP{
public:
    process_code_WLP();
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
    double getDEFECT_SUM() const;
    void setDEFECT_SUM(double value);
    double getEXCLUDE_YIELD_QTY_SUM() const;
    void setEXCLUDE_YIELD_QTY_SUM(double value);
    double getVield() const;
    void setVield(double value);
};
class rework_text_type_WLP{
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


class defect_worst_type_WLP{
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

class worst_search_WLP_th :public QThread
{
    Q_OBJECT
public:
    worst_search_WLP_th();
    QSqlDatabase ms_mesdb;
    QSqlDatabase my_mesdb;
    QVector<process_code_WLP> daily_worstcpsprocesslist;
    QVector<process_code_WLP> accumulate_worstcpsprocesslist;
    QVector<rework_text_type_WLP> rework_worstcpsprocesslist;
    QVector<defect_worst_type_WLP> defect_worstcpsprocesslist;
    QStringList DEFECT_ITEM_LIST;
    QStringList REWORK_ITEM_LIST;
    QStringList PROBE_ITEM_LIST;
    QStringList EXTERIOR_DEFECT_ITEM_LIST;
    QDate select_date;
    QDate getSelect_date() const;
    void setSelect_date(const QDate &value);

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
    double total_brigit_pad_worst;

    double total_Rework_paticle;
    double total_Jobmiss_defect;
    double total_Jobmiss_eatching;
    double total_Jobmiss_light;
    double total_Jobmiss_probe;

    double exterior_pattenpaticle_vaild;
    double exterior_padpaticle_vaild;
    double exterior_sindiseatching_vaild;
    double exterior_etcpaticle_vaild;
    double exterior_brigitpadworst_vaild;

    double exterior_vaild;
    double exterior_worst_count;


    double getDaily_totalvild() const;
    void setDaily_totalvild(double value);

    double getAccumulate_totalvild() const;
    void setAccumulate_totalvild(double value);

    double getLast_probe_vild_daily() const;
    void setLast_probe_vild_daily(double value);

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

    double getTotal_brigit_pad_worst() const;
    void setTotal_brigit_pad_worst(double value);

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

    double getExterior_vaild() const;
    void setExterior_vaild(double value);

    double getExterior_worst_count() const;
    void setExterior_worst_count(double value);

private:
    void run();
signals:
    void sig_excel_work();
    void sig_debug_output(QString str);
};

#endif // WORST_SEARCH_WLP_TH_H
