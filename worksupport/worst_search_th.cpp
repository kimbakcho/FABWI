#include "worst_search_th.h"

worst_search_th::worst_search_th()
{
    QString msdb_name = QString("MS_MESDB_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString mydb_name = QString("MY_MESDB_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    ms_mesdb = QSqlDatabase::addDatabase("QODBC",msdb_name);
    QString serverinfo = "DRIVER={SQL Server};Server=10.20.10.221;Database=MESDB;Uid=fabview;Port=1433;Pwd=fabview";
    ms_mesdb.setDatabaseName(serverinfo);
    QString configini_str = qApp->applicationDirPath()+"/server.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    QString server_ip = settings.value("serverip").toString();
    QString db_port = settings.value("dbport").toString();
    QString ftp_port = settings.value("ftpport").toString();
    settings.endGroup();

    if(!my_mesdb.contains(mydb_name)){
       my_mesdb = QSqlDatabase::addDatabase("QMYSQL",mydb_name);
       my_mesdb.setHostName(server_ip);
       my_mesdb.setPort(db_port.toInt());
       my_mesdb.setUserName(DBID);
       my_mesdb.setPassword(DBPW);
       my_mesdb.setDatabaseName("FAB");
    }else {
       my_mesdb = QSqlDatabase::database(mydb_name);
    }
    if(!my_mesdb.isOpen()){
        if(!my_mesdb.open()){
             qDebug()<<my_mesdb.lastError().text();
        }
    }
    if(!ms_mesdb.open()){
        qDebug()<<"fasle";
        qDebug()<<ms_mesdb.lastError().text();
    }else {
        qDebug()<<"open";
    }

    DEFECT_ITEM_LIST.append(tr("workerfail(probe)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(eatching)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(light)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(deposition)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(output)"));

    DEFECT_ITEM_LIST.append(tr("machinefail(probe)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(eatching)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(light)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(deposition)"));

    DEFECT_ITEM_LIST.append(tr("paticle"));
    DEFECT_ITEM_LIST.append(tr("padpaticle"));
    DEFECT_ITEM_LIST.append(tr("etcpaticle"));
    DEFECT_ITEM_LIST.append(tr("pattenpaticle"));
    DEFECT_ITEM_LIST.append(tr("sin_diseatching_worst"));
    DEFECT_ITEM_LIST.append(tr("brigit_pad_worst"));

    EXTERIOR_DEFECT_ITEM_LIST.append(tr("paticle"));
    EXTERIOR_DEFECT_ITEM_LIST.append(tr("padpaticle"));
    EXTERIOR_DEFECT_ITEM_LIST.append(tr("etcpaticle"));
    EXTERIOR_DEFECT_ITEM_LIST.append(tr("pattenpaticle"));
    EXTERIOR_DEFECT_ITEM_LIST.append(tr("sin_diseatching_worst"));
    EXTERIOR_DEFECT_ITEM_LIST.append(tr("brigit_pad_worst"));

    REWORK_ITEM_LIST.append(tr("jobmiss(probe)"));//작업 미스(프로브)
    REWORK_ITEM_LIST.append(tr("jobmiss(eatching)"));//작업 미스(성막)
    REWORK_ITEM_LIST.append(tr("jobmiss(light)"));//작업 미스(노광)
    REWORK_ITEM_LIST.append(tr("jobmiss(deposition)"));//작업 미스(성막)
    REWORK_ITEM_LIST.append(tr("paticle"));//이물

    REWORK_ITEM_LIST.append(tr("metalcdhigh"));//메탈CD큼
    REWORK_ITEM_LIST.append(tr("metalcdlow"));//메탈CD작음
    REWORK_ITEM_LIST.append(tr("prcdhigh"));//PRCD큼
    REWORK_ITEM_LIST.append(tr("prcdlow"));//PRCD작음

    PROBE_ITEM_LIST.append("DP001");//OS불량
    PROBE_ITEM_LIST.append("DP002");//IL불량
    PROBE_ITEM_LIST.append("DP003");//저주파
    PROBE_ITEM_LIST.append("DP004");//고주파
    PROBE_ITEM_LIST.append("DP005");//BW 불량
    PROBE_ITEM_LIST.append("DP006");//LIMIT 불량
//    PROBE_ITEM_LIST.append("DP007");//DF 큼
    PROBE_ITEM_LIST.append("DP008");//VSWR불량

    CSP_LIMIT_PRODUCT_LIST.append("X897WYF-TXBCG");
    CSP_LIMIT_PRODUCT_LIST.append("X897WYF-TXBDG");
    CSP_LIMIT_PRODUCT_LIST.append("X897BYF-TXBCG");

    last_probe_vild_daily = 0;
    exterior_vaild=0;
    exterior_worst_count =0;
}

QVector<csp_limit_product_type> worst_search_th::getCSP_Limit_product_item_list() const
{
    return CSP_Limit_product_item_list;
}

void worst_search_th::setCSP_Limit_product_item_list(const QVector<csp_limit_product_type> &value)
{
    CSP_Limit_product_item_list = value;
}

double worst_search_th::getExterior_brigitpadworst_vaild() const
{
    return exterior_brigitpadworst_vaild;
}

void worst_search_th::setExterior_brigitpadworst_vaild(double value)
{
    exterior_brigitpadworst_vaild = value;
}

double worst_search_th::getExterior_etcpaticle_vaild() const
{
    return exterior_etcpaticle_vaild;
}

void worst_search_th::setExterior_etcpaticle_vaild(double value)
{
    exterior_etcpaticle_vaild = value;
}

double worst_search_th::getExterior_sindiseatching_vaild() const
{
    return exterior_sindiseatching_vaild;
}

void worst_search_th::setExterior_sindiseatching_vaild(double value)
{
    exterior_sindiseatching_vaild = value;
}

double worst_search_th::getExterior_padpaticle_vaild() const
{
    return exterior_padpaticle_vaild;
}

void worst_search_th::setExterior_padpaticle_vaild(double value)
{
    exterior_padpaticle_vaild = value;
}

double worst_search_th::getExterior_pattenpaticle_vaild() const
{
    return exterior_pattenpaticle_vaild;
}

void worst_search_th::setExterior_pattenpaticle_vaild(double value)
{
    exterior_pattenpaticle_vaild = value;
}


double worst_search_th::getExterior_worst_count() const
{
    return exterior_worst_count;
}

void worst_search_th::setExterior_worst_count(double value)
{
    exterior_worst_count = value;
}

double worst_search_th::getExterior_vaild() const
{
    return exterior_vaild;
}

void worst_search_th::setExterior_vaild(double value)
{
    exterior_vaild = value;
}

double worst_search_th::getTotal_brigit_pad_worst() const
{
    return total_brigit_pad_worst;
}

void worst_search_th::setTotal_brigit_pad_worst(double value)
{
    total_brigit_pad_worst = value;
}

double worst_search_th::getTotal_sin_diseatching_worst() const
{
    return total_sin_diseatching_worst;
}

void worst_search_th::setTotal_sin_diseatching_worst(double value)
{
    total_sin_diseatching_worst = value;
}

QVector<defect_worst_type> worst_search_th::getDefect_worstcpsprocesslist() const
{
    return defect_worstcpsprocesslist;
}

void worst_search_th::setDefect_worstcpsprocesslist(const QVector<defect_worst_type> &value)
{
    defect_worstcpsprocesslist = value;
}

QVector<rework_text_type> worst_search_th::getRework_worstcpsprocesslist() const
{
    return rework_worstcpsprocesslist;
}

void worst_search_th::setRework_worstcpsprocesslist(const QVector<rework_text_type> &value)
{
    rework_worstcpsprocesslist = value;
}

double worst_search_th::getRSM_accumulate_totalvild() const
{
    return RSM_accumulate_totalvild;
}

void worst_search_th::setRSM_accumulate_totalvild(double value)
{
    RSM_accumulate_totalvild = value;
}

double worst_search_th::getRSM_last_probevild() const
{
    return RSM_last_probevild;
}

void worst_search_th::setRSM_last_probevild(double value)
{
    RSM_last_probevild = value;
}

double worst_search_th::getRSM_daily_totalvild() const
{
    return RSM_daily_totalvild;
}

void worst_search_th::setRSM_daily_totalvild(double value)
{
    RSM_daily_totalvild = value;
}

double worst_search_th::getDPX_accumulate_totalvild() const
{
    return DPX_accumulate_totalvild;
}

void worst_search_th::setDPX_accumulate_totalvild(double value)
{
    DPX_accumulate_totalvild = value;
}

double worst_search_th::getDPX_last_probevild() const
{
    return DPX_last_probevild;
}

void worst_search_th::setDPX_last_probevild(double value)
{
    DPX_last_probevild = value;
}

double worst_search_th::getDPX_daily_totalvild() const
{
    return DPX_daily_totalvild;
}

void worst_search_th::setDPX_daily_totalvild(double value)
{
    DPX_daily_totalvild = value;
}

double worst_search_th::getSingle_last_probevild() const
{
    return single_last_probevild;
}

void worst_search_th::setSingle_last_probevild(double value)
{
    single_last_probevild = value;
}

double worst_search_th::getSingle_accumulate_totalvild() const
{
    return single_accumulate_totalvild;
}

void worst_search_th::setSingle_accumulate_totalvild(double value)
{
    single_accumulate_totalvild = value;
}

double worst_search_th::getSingle_daily_totalvild() const
{
    return single_daily_totalvild;
}

void worst_search_th::setSingle_daily_totalvild(double value)
{
    single_daily_totalvild = value;
}

double worst_search_th::getLast_probe_vild_daily() const
{
    return last_probe_vild_daily;
}

void worst_search_th::setLast_probe_vild_daily(double value)
{
    last_probe_vild_daily = value;
}

double worst_search_th::getTotal_prcdlow() const
{
    return total_prcdlow;
}

void worst_search_th::setTotal_prcdlow(double value)
{
    total_prcdlow = value;
}

double worst_search_th::getTotal_prcdhigh() const
{
    return total_prcdhigh;
}

void worst_search_th::setTotal_prcdhigh(double value)
{
    total_prcdhigh = value;
}

double worst_search_th::getTotal_metallow() const
{
    return total_metallow;
}

void worst_search_th::setTotal_metallow(double value)
{
    total_metallow = value;
}

double worst_search_th::getTotal_metalhigh() const
{
    return total_metalhigh;
}

void worst_search_th::setTotal_metalhigh(double value)
{
    total_metalhigh = value;
}

double worst_search_th::getTotal_Jobmiss_probe() const
{
    return total_Jobmiss_probe;
}

void worst_search_th::setTotal_Jobmiss_probe(double value)
{
    total_Jobmiss_probe = value;
}

double worst_search_th::getTotal_Jobmiss_light() const
{
    return total_Jobmiss_light;
}

void worst_search_th::setTotal_Jobmiss_light(double value)
{
    total_Jobmiss_light = value;
}

double worst_search_th::getTotal_Jobmiss_eatching() const
{
    return total_Jobmiss_eatching;
}

void worst_search_th::setTotal_Jobmiss_eatching(double value)
{
    total_Jobmiss_eatching = value;
}

double worst_search_th::getTotal_Jobmiss_defect() const
{
    return total_Jobmiss_defect;
}

void worst_search_th::setTotal_Jobmiss_defect(double value)
{
    total_Jobmiss_defect = value;
}

double worst_search_th::getTotal_Rework_paticle() const
{
    return total_Rework_paticle;
}

void worst_search_th::setTotal_Rework_paticle(double value)
{
    total_Rework_paticle = value;
}

double worst_search_th::getTotal_Defectpaticle() const
{
    return total_Defectpaticle;
}

void worst_search_th::setTotal_Defectpaticle(double value)
{
    total_Defectpaticle = value;
}

double worst_search_th::getTotal_Pattenpaticle() const
{
    return total_Pattenpaticle;
}

void worst_search_th::setTotal_Pattenpaticle(double value)
{
    total_Pattenpaticle = value;
}

double worst_search_th::getTotal_Etcpaticle() const
{
    return total_Etcpaticle;
}

void worst_search_th::setTotal_Etcpaticle(double value)
{
    total_Etcpaticle = value;
}

double worst_search_th::getTotal_Padpaticle() const
{
    return total_Padpaticle;
}

void worst_search_th::setTotal_Padpaticle(double value)
{
    total_Padpaticle = value;
}

double worst_search_th::getTotal_Machinefail_defect() const
{
    return total_Machinefail_defect;
}

void worst_search_th::setTotal_Machinefail_defect(double value)
{
    total_Machinefail_defect = value;
}

double worst_search_th::getTotal_Machinefail_light() const
{
    return total_Machinefail_light;
}

void worst_search_th::setTotal_Machinefail_light(double value)
{
    total_Machinefail_light = value;
}

double worst_search_th::getTotal_Machinefail_eatching() const
{
    return total_Machinefail_eatching;
}

void worst_search_th::setTotal_Machinefail_eatching(double value)
{
    total_Machinefail_eatching = value;
}

double worst_search_th::getTotal_Machinefail_probe() const
{
    return total_Machinefail_probe;
}

void worst_search_th::setTotal_Machinefail_probe(double value)
{
    total_Machinefail_probe = value;
}

double worst_search_th::getTotal_Workerfail_output() const
{
    return total_Workerfail_output;
}

void worst_search_th::setTotal_Workerfail_output(double value)
{
    total_Workerfail_output = value;
}

double worst_search_th::getTotal_Workerfail_defect() const
{
    return total_Workerfail_defect;
}

void worst_search_th::setTotal_Workerfail_defect(double value)
{
    total_Workerfail_defect = value;
}

double worst_search_th::getTotal_Workerfail_light() const
{
    return total_Workerfail_light;
}

void worst_search_th::setTotal_Workerfail_light(double value)
{
    total_Workerfail_light = value;
}

double worst_search_th::getTotal_Workerfail_eatching() const
{
    return total_Workerfail_eatching;
}

void worst_search_th::setTotal_Workerfail_eatching(double value)
{
    total_Workerfail_eatching = value;
}

double worst_search_th::getTotal_Workerfail_probe() const
{
    return total_Workerfail_probe;
}

void worst_search_th::setTotal_Workerfail_probe(double value)
{
    total_Workerfail_probe = value;
}

double worst_search_th::getTotal_DP008_worst() const
{
    return total_DP008_worst;
}

void worst_search_th::setTotal_DP008_worst(double value)
{
    total_DP008_worst = value;
}

double worst_search_th::getTotal_DP006_worst() const
{
    return total_DP006_worst;
}

void worst_search_th::setTotal_DP006_worst(double value)
{
    total_DP006_worst = value;
}

double worst_search_th::getTotal_DP005_worst() const
{
    return total_DP005_worst;
}

void worst_search_th::setTotal_DP005_worst(double value)
{
    total_DP005_worst = value;
}

double worst_search_th::getTotal_DP004_worst() const
{
    return total_DP004_worst;
}

void worst_search_th::setTotal_DP004_worst(double value)
{
    total_DP004_worst = value;
}

double worst_search_th::getTotal_DP003_worst() const
{
    return total_DP003_worst;
}

void worst_search_th::setTotal_DP003_worst(double value)
{
    total_DP003_worst = value;
}

double worst_search_th::getTotal_DP001_worst() const
{
    return total_DP001_worst;
}

void worst_search_th::setTotal_DP001_worst(double value)
{
    total_DP001_worst = value;
}

double worst_search_th::getWorst_sum() const
{
    return worst_sum;
}

void worst_search_th::setWorst_sum(double value)
{
    worst_sum = value;
}

double worst_search_th::getAccumulate_totalvild() const
{
    return accumulate_totalvild;
}

void worst_search_th::setAccumulate_totalvild(double value)
{
    accumulate_totalvild = value;
}

double worst_search_th::getDaily_totalvild() const
{
    return daily_totalvild;
}

void worst_search_th::setDaily_totalvild(double value)
{
    daily_totalvild = value;
}

QDate worst_search_th::getSelect_date() const
{
    return select_date;
}

void worst_search_th::setSelect_date(const QDate &value)
{
    select_date = value;
}

void worst_search_th::run()
{
    daily_worstcpsprocesslist.clear();
    accumulate_worstcpsprocesslist.clear();
    single_worstcpsprocesslist.clear();
    single_accumulate_worstcpsprocesslist.clear();
    DPX_worstcpsprocesslist.clear();
    DPX_accumulate_worstcpsprocesslist.clear();
    RSM_worstcpsprocesslist.clear();
    RSM_accumulate_worstcpsprocesslist.clear();
    rework_worstcpsprocesslist.clear();
    defect_worstcpsprocesslist.clear();
    CSP_Limit_product_item_list.clear();

    exterior_pattenpaticle_vaild=0;
    exterior_padpaticle_vaild=0;
    exterior_sindiseatching_vaild=0;
    exterior_etcpaticle_vaild=0;
    exterior_brigitpadworst_vaild=0;

    QSqlQuery my_query(my_mesdb);
    QSqlQuery ms_query(ms_mesdb);
    QString start_date_str = select_date.toString("yyyyMMdd")+"080000";
    QString end_date_str = select_date.addDays(1).toString("yyyyMMdd")+"075959";

#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\';")
                                          .arg(start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName());
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
            if(data.getName() == tr("lastprobe_vaild")){
                last_probe_vild_daily = data.getVield()*100.0;
                last_probe_vild_daily = roundf(last_probe_vild_daily * 100) / 100;
            }else if(data.getName() == tr("exterior")){
                exterior_vaild = data.getVield()*100.0;
                exterior_vaild = roundf(exterior_vaild * 100) / 100;
//                exterior_worst_count = data.getDEFECT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM();
            }
        }


        daily_worstcpsprocesslist.append(data);
    }
    daily_totalvild = 1.0;
    for(int i=0;i<daily_worstcpsprocesslist.count();i++){
        if(daily_worstcpsprocesslist.at(i).getVield() != 0){
            daily_totalvild *= daily_worstcpsprocesslist.at(i).getVield();
        }
    }
    daily_totalvild *=100.0;
    daily_totalvild = roundf(daily_totalvild * 100) / 100;
    emit sig_debug_output(QString("daily_total vaild = %1").arg(daily_totalvild));
#else
    last_probe_vild_daily = 89.12;
    daily_totalvild = 89.61;
#endif
    QDate firstday(select_date.year(),select_date.month(),1);
    QString accumulate_start_date_str = firstday.toString("yyyyMMdd")+"080000";
#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());
        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\';")
                                          .arg(accumulate_start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName());
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
        }
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);

        accumulate_worstcpsprocesslist.append(data);
    }
    accumulate_totalvild = 1.0;
    for(int i=0;i<accumulate_worstcpsprocesslist.count();i++){
        if(accumulate_worstcpsprocesslist.at(i).getVield() != 0){
            accumulate_totalvild *= accumulate_worstcpsprocesslist.at(i).getVield();
        }
    }
    accumulate_totalvild *=100.0;
    accumulate_totalvild = roundf(accumulate_totalvild * 100) / 100;
    emit sig_debug_output(QString("accumulate_total = %1").arg(accumulate_totalvild));
#else
    accumulate_totalvild = 93.8;
#endif

#ifdef REAL_QUERY
    QString DEFECT_QTY_SUM_query = QString("SELECT SUM(DEFECT_QTY) DEFECT_SUM "
                                           "FROM [V_FAB_DEFECT_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP' "
                                           "AND EXCLUDE_YIELD_FLAG <> 'Y';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(DEFECT_QTY_SUM_query);
    ms_query.exec(DEFECT_QTY_SUM_query);

    ms_query.next();
    double defect_sum = ms_query.value("DEFECT_SUM").toDouble();

    QString REWORK_SUM_query = QString("SELECT SUM(REWORK_CHIP_QTY)REWORK_SUM "
                                           "FROM [V_FAB_REWORK_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(REWORK_SUM_query);
    ms_query.exec(REWORK_SUM_query);
    ms_query.next();
    double rework_sum = ms_query.value("REWORK_SUM").toDouble();

    QString PROBE_SUM_query = QString("SELECT SUM(PROBE_INSP_QTY) PROBE_INSP_SUM "
                                           "FROM [V_FAB_PROBE_INSP_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(PROBE_SUM_query);
    ms_query.exec(PROBE_SUM_query);
    ms_query.next();
    double probe_sum = ms_query.value("PROBE_INSP_SUM").toDouble();

    worst_sum = defect_sum+rework_sum+probe_sum;

    DEFECT_QTY_SUM_query = QString("SELECT SUM(DEFECT_QTY) DEFECT_SUM "
                                           "FROM [V_FAB_DEFECT_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP' "
                                           "AND OPERATION_SHORT_NAME = '%3' "
                                           "AND REPROCESS_FLAG = 'N' "
                                           "AND EXCLUDE_YIELD_FLAG <> 'Y';")
                                           .arg(start_date_str).arg(end_date_str).arg(tr("exterior"));
    emit sig_debug_output(DEFECT_QTY_SUM_query);
    ms_query.exec(DEFECT_QTY_SUM_query);

    ms_query.next();
    defect_sum = ms_query.value("DEFECT_SUM").toDouble();

    REWORK_SUM_query = QString("SELECT SUM(REWORK_CHIP_QTY)REWORK_SUM "
                                           "FROM [V_FAB_REWORK_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND OPERATION_SHORT_NAME = '%3' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str).arg(tr("exterior"));
    emit sig_debug_output(REWORK_SUM_query);
    ms_query.exec(REWORK_SUM_query);
    ms_query.next();
    rework_sum = ms_query.value("REWORK_SUM").toDouble();

    PROBE_SUM_query = QString("SELECT SUM(PROBE_INSP_QTY) PROBE_INSP_SUM "
                                           "FROM [V_FAB_PROBE_INSP_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND OPERATION_SHORT_NAME = '%3' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str).arg(tr("exterior"));
    emit sig_debug_output(PROBE_SUM_query);
    ms_query.exec(PROBE_SUM_query);
    ms_query.next();
    probe_sum = ms_query.value("PROBE_INSP_SUM").toDouble();

    exterior_worst_count = defect_sum+rework_sum+probe_sum;



#else
    worst_sum = 1153931;
#endif
    emit sig_debug_output(QString("worst_sum = %1").arg(worst_sum));
    QString test = QString("%1").arg((int)worst_sum);
    qDebug()<<test;


#ifdef REAL_QUERY

    total_DP001_worst=0;
    total_DP003_worst=0;
    total_DP004_worst=0;
    total_DP005_worst=0;
    total_DP006_worst=0;
    total_DP008_worst=0;
    for(int j=0;j<PROBE_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT SUM(PROBE_INSP_QTY)PROBE_INSP_SUM "
                                               "FROM [V_FAB_PROBE_INSP_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND PROBE_INSP_CODE = '%3';").arg(start_date_str).arg(end_date_str)
                                               .arg(PROBE_ITEM_LIST.at(j));

            ms_query.exec(worstcount_query);
            emit sig_debug_output(worstcount_query);

            qDebug()<<worstcount_query;
            if(ms_query.next()){
                int count = ms_query.value("PROBE_INSP_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                if(count !=0){
                vaild_rate = ((count/temp2)*temp1);
                    if(PROBE_ITEM_LIST.at(j)=="DP001"){     //OS 불량 .
                          total_DP001_worst = vaild_rate;
                    }else if(PROBE_ITEM_LIST.at(j)=="DP003"){  //저주파 .
                          total_DP003_worst = vaild_rate;
                    }else if(PROBE_ITEM_LIST.at(j)=="DP004"){  //고주파 .
                          total_DP004_worst = vaild_rate;
                    }else if(PROBE_ITEM_LIST.at(j)=="DP005"){  //BW 불량 .
                          total_DP005_worst = vaild_rate;
                    }else if(PROBE_ITEM_LIST.at(j)=="DP006"){ //LIMIT 불량
                          total_DP006_worst = vaild_rate;
                    }else if(PROBE_ITEM_LIST.at(j)=="DP008"){  //VSWR불량
                          total_DP008_worst = vaild_rate;
                    }
                }
            }
        }
#else
    total_DP001_worst=0.13;
    total_DP003_worst=0.48;
    total_DP004_worst=0.21;
    total_DP005_worst=0.03;
    total_DP006_worst=0.52;
    total_DP008_worst=0.22;
#endif
    qDebug()<<"p OS DP001 = "<<RoundOff(total_DP001_worst,2);
    qDebug()<<"p lowfre DP003 = "<<RoundOff(total_DP003_worst,2);
    qDebug()<<"p highfre DP004 = "<<RoundOff(total_DP004_worst,2);
    qDebug()<<"p bw DP005 = "<<RoundOff(total_DP005_worst,2);
    qDebug()<<"p limit DP006 = "<<RoundOff(total_DP006_worst,2);
    qDebug()<<"p vswr DP008 = "<<RoundOff(total_DP008_worst,2);

    total_Workerfail_probe=0;
    total_Workerfail_eatching=0;
    total_Workerfail_light=0;
    total_Workerfail_defect=0;
    total_Workerfail_output=0;
    total_Machinefail_probe=0;
    total_Machinefail_eatching=0;
    total_Machinefail_light=0;
    total_Machinefail_defect=0;
    total_Padpaticle=0;
    total_Etcpaticle=0;
    total_Pattenpaticle=0;
    total_Defectpaticle=0;
#ifdef REAL_QUERY
        for(int j=0;j<DEFECT_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT SUM(DEFECT_QTY)DEFECT_SUM "
                                               "FROM [V_FAB_DEFECT_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                               "AND DEFECT_NAME = '%4'")
                                                .arg(start_date_str).arg(end_date_str)
                                                .arg(DEFECT_ITEM_LIST.at(j));

            ms_query.exec(worstcount_query);
            emit sig_debug_output(worstcount_query);
            qDebug()<<worstcount_query;
            if(ms_query.next()){
                int count = ms_query.value("DEFECT_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                if(count!=0){
                    vaild_rate = ((count/temp2)*temp1);

                    if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(probe)")){     //작업자 파손(프로브)
                        total_Workerfail_probe = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(eatching)")){  //작업자 파손(에칭)
                        total_Workerfail_eatching = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(light)")){  //작업자 파손(노광)
                        total_Workerfail_light = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(deposition)")){  //작업자 파손(성막)
                        total_Workerfail_defect = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(output)")){ //작업자 파손(출고)
                        total_Workerfail_output = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(probe)")){  //설비파손(프로브)
                        total_Machinefail_probe = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(eatching)")){  //설비파손(에칭)
                        total_Machinefail_eatching = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(light)")){  //설비파손(노광)
                        total_Machinefail_light = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(deposition)")){  //설비파손(성막)
                        total_Machinefail_defect = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("padpaticle")){  //패트 이물
                        total_Padpaticle = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("etcpaticle")){  //이외 이물
                        total_Etcpaticle = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("pattenpaticle")){  //패턴 이물
                        total_Pattenpaticle = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("paticle")){  //이물
                        total_Defectpaticle = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("sin_diseatching_worst")){ //sin 미에칭
                        total_sin_diseatching_worst = vaild_rate;
                    }else if(DEFECT_ITEM_LIST.at(j)==tr("brigit_pad_worst")){//브릿지 패드
                        total_brigit_pad_worst = vaild_rate;
                    }
                }
            }
        }
#else
    total_Workerfail_probe=0;
    total_Workerfail_eatching=0;
    total_Workerfail_light=0;
    total_Workerfail_defect=0.821435;
    total_Workerfail_output=0;
    total_Machinefail_probe=0;
    total_Machinefail_eatching=0.0530336;
    total_Machinefail_light=0.416291;
    total_Machinefail_defect=0.968399;
    total_Padpaticle= 0.152384;
    total_Etcpaticle=0.0363852;
    total_Pattenpaticle=0.0155499;
    total_Defectpaticle=0.02;
    total_sin_diseatching_worst = 0.21;
    total_brigit_pad_worst = 0.13;
#endif
    qDebug()<<"d total_Workerfail_probe = "<<total_Workerfail_probe ;
    qDebug()<<"d total_Workerfail_eatching = "<<total_Workerfail_eatching ;
    qDebug()<<"d total_Workerfail_light = "<<total_Workerfail_light ;
    qDebug()<<"d total_Workerfail_defect = "<<total_Workerfail_defect ;
    qDebug()<<"d total_Workerfail_output = "<<total_Workerfail_output ;
    qDebug()<<"d total_Machinefail_probe = "<<total_Machinefail_probe ;
    qDebug()<<"d total_Machinefail_eatching = "<<total_Machinefail_eatching ;
    qDebug()<<"d total_Machinefail_light = "<<total_Machinefail_light ;
    qDebug()<<"d total_Machinefail_defect = "<<total_Machinefail_defect ;
    qDebug()<<"d total_Padpaticle = "<<total_Padpaticle ;
    qDebug()<<"d total_Etcpaticle = "<<total_Etcpaticle ;
    qDebug()<<"d total_Pattenpaticle = "<<total_Pattenpaticle ;
    qDebug()<<"d total_Defectpaticle = "<<total_Defectpaticle ;


    total_Rework_paticle=0;
    total_Jobmiss_defect=0;
    total_Jobmiss_eatching=0;
    total_Jobmiss_light=0;
    total_Jobmiss_probe=0;
    total_metalhigh = 0;
    total_metallow = 0;
    total_prcdhigh = 0;
    total_prcdlow = 0;
#ifdef REAL_QUERY
        for(int j=0;j<REWORK_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT  SUM(REWORK_CHIP_QTY)REWORK_SUM "
                                               "FROM [V_FAB_REWORK_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND REWORK_NAME = '%4' ")
                                                .arg(start_date_str).arg(end_date_str)
                                                .arg(REWORK_ITEM_LIST.at(j));
            ms_query.exec(worstcount_query);
            emit sig_debug_output(worstcount_query);
            if(ms_query.next()){
                int count = ms_query.value("REWORK_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                if(count!=0){
                    vaild_rate = ((count/temp2)*temp1);
                    if(REWORK_ITEM_LIST.at(j)==tr("paticle")){     //이물
                        total_Rework_paticle = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(deposition)")){ //작업 미스(성막)
                        total_Jobmiss_defect = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(eatching)")){ //작업 미스(에칭)
                        total_Jobmiss_eatching = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(light)")){ //작업 미스(성막)
                        total_Jobmiss_light = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(probe)")){ //작업미스(프로브)
                        total_Jobmiss_probe = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("metalcdhigh")){
                        total_metalhigh = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("metalcdlow")){
                        total_metallow = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("prcdhigh")){
                        total_prcdhigh = vaild_rate;
                    }else if(REWORK_ITEM_LIST.at(j)==tr("prcdlow")){
                        total_prcdlow = vaild_rate;
                    }
                }
            }
        }

#else
    total_Rework_paticle=1.27281;
    total_Jobmiss_defect=0;
    total_Jobmiss_eatching=0;
    total_Jobmiss_light=0.0656752;
    total_Jobmiss_probe=0;
#endif
    qDebug()<<"r total_Rework_paticle = "<<total_Rework_paticle;
    qDebug()<<"r total_Jobmiss_defect = "<<total_Jobmiss_defect;
    qDebug()<<"r total_Jobmiss_eatching = "<<total_Jobmiss_eatching;
    qDebug()<<"r total_Jobmiss_light = "<<total_Jobmiss_light;
    qDebug()<<"r total_Jobmiss_probe = "<<total_Jobmiss_probe;

    single_daily_totalvild = 0;
#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("MOLD").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
            if(data.getName() == tr("lastprobe_vaild")){
                single_last_probevild = data.getVield()*100.0;
                single_last_probevild = roundf(single_last_probevild * 100) / 100;
            }
        }else {
            single_last_probevild = 100.0;
        }

        single_worstcpsprocesslist.append(data);
    }
    single_daily_totalvild = 1.0;

    for(int i=0;i<single_worstcpsprocesslist.count();i++){
        if(single_worstcpsprocesslist.at(i).getVield() != 0){
            single_daily_totalvild *= single_worstcpsprocesslist.at(i).getVield();
        }
    }
    single_daily_totalvild *= 100.0;
    single_daily_totalvild = roundf(single_daily_totalvild * 100) / 100;
    emit sig_debug_output(QString("single_daily_totalvild = %1").arg(single_daily_totalvild));
#else
    single_last_probevild = 92.3;
    single_daily_totalvild = 93.4;
#endif


#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(accumulate_start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("MOLD").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
        }

        single_accumulate_worstcpsprocesslist.append(data);
    }
    single_accumulate_totalvild = 1.0;
    for(int i=0;i<single_accumulate_worstcpsprocesslist.count();i++){
        if(single_accumulate_worstcpsprocesslist.at(i).getVield() != 0){
            single_accumulate_totalvild *= single_accumulate_worstcpsprocesslist.at(i).getVield();
        }
    }
    single_accumulate_totalvild *= 100.0;
    single_accumulate_totalvild = roundf(single_accumulate_totalvild * 100) / 100;
    emit sig_debug_output(QString("single_accumulate_totalvild = %1").arg(single_accumulate_totalvild));
#else
    single_accumulate_totalvild = 93.4;
#endif

    DPX_daily_totalvild = 0;
#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("SAWDPX").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
            if(data.getName() == tr("lastprobe_vaild")){
                DPX_last_probevild = data.getVield()*100.0;
                DPX_last_probevild = roundf(DPX_last_probevild * 100) / 100;
            }
        }else {
            DPX_last_probevild = 100.0;
        }

        DPX_worstcpsprocesslist.append(data);
    }
    DPX_daily_totalvild = 1.0;
    for(int i=0;i<DPX_worstcpsprocesslist.count();i++){
        if(DPX_worstcpsprocesslist.at(i).getVield() != 0){
            DPX_daily_totalvild *= DPX_worstcpsprocesslist.at(i).getVield();
        }
    }
    DPX_daily_totalvild *= 100.0;
    DPX_daily_totalvild = roundf(DPX_daily_totalvild * 100) / 100;

    emit sig_debug_output(QString("DPX_daily_totalvild = %1").arg(DPX_daily_totalvild));
#else
    DPX_last_probevild = 92.03;
    DPX_daily_totalvild = 92.04;
#endif

#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(accumulate_start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("SAWDPX").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
        }

        DPX_accumulate_worstcpsprocesslist.append(data);
    }
    DPX_accumulate_totalvild = 1.0;
    for(int i=0;i<DPX_accumulate_worstcpsprocesslist.count();i++){
        if(DPX_accumulate_worstcpsprocesslist.at(i).getVield() != 0){
            DPX_accumulate_totalvild *= DPX_accumulate_worstcpsprocesslist.at(i).getVield();
        }
    }
    DPX_accumulate_totalvild *= 100.0;
    DPX_accumulate_totalvild = roundf(DPX_accumulate_totalvild * 100) / 100;
    emit sig_debug_output(QString("DPX_accumulate_totalvild = %1").arg(DPX_accumulate_totalvild));
#else
    DPX_accumulate_totalvild = 93.4;
#endif


    RSM_daily_totalvild = 0;
#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("RSM").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
            if(data.getName() == tr("lastprobe_vaild")){
                RSM_last_probevild = data.getVield()*100.0;
                RSM_last_probevild = roundf(RSM_last_probevild * 100) / 100;
            }
        }else {
            RSM_last_probevild = 100.0;
        }

        RSM_worstcpsprocesslist.append(data);
    }
    RSM_daily_totalvild = 1.0;
    for(int i=0;i<RSM_worstcpsprocesslist.count();i++){
        if(RSM_worstcpsprocesslist.at(i).getVield() != 0){
            RSM_daily_totalvild *= RSM_worstcpsprocesslist.at(i).getVield();
        }
    }
    RSM_daily_totalvild *= 100.0;
    RSM_daily_totalvild = roundf(RSM_daily_totalvild * 100) / 100;
    emit sig_debug_output(QString("RSM_daily_totalvild = %1").arg(RSM_daily_totalvild));
#else
    RSM_last_probevild = 92.03;
    RSM_daily_totalvild = 92.04;
#endif
#ifdef REAL_QUERY
    my_query.exec("select * from MES_Worst_CSP_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_FAB_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\' "
                                          "AND MATERIAL_CATEGORY = '%4' "
                                          "AND MATERIAL_TYPE = '%5'")
                                          .arg(accumulate_start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName())
                                          .arg("RSM").arg("HALB");
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        emit sig_debug_output(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        if(data.getOUTPUT_SUM() != 0){
            data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());
        }

        RSM_accumulate_worstcpsprocesslist.append(data);
    }
    RSM_accumulate_totalvild = 1.0;
    for(int i=0;i<RSM_accumulate_worstcpsprocesslist.count();i++){
        if(RSM_accumulate_worstcpsprocesslist.at(i).getVield() != 0){
            RSM_accumulate_totalvild *= RSM_accumulate_worstcpsprocesslist.at(i).getVield();
        }
    }
    RSM_accumulate_totalvild *= 100.0;
    RSM_accumulate_totalvild = roundf(RSM_accumulate_totalvild * 100) / 100;
#else
    RSM_accumulate_totalvild = 93.3;
#endif

    emit sig_debug_output(QString("RSM_accumulate_totalvild = %1").arg(RSM_accumulate_totalvild));
#ifdef REAL_QUERY

    QString rework_type_txt = QString("SELECT REWORK_NAME,SUM(REWORK_CHIP_QTY) AS rework_qty "
                                      "FROM [V_FAB_REWORK_LOTS] "
                                      "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                      "AND LOT_TYPE = 'A' "
                                      "AND MATERIAL_GROUP = 'CSP' "
                                      "group by REWORK_NAME "
                                      "order by rework_qty desc;").arg(start_date_str).arg(end_date_str);

    ms_query.exec(rework_type_txt);
    QSqlQuery ms_query_2(ms_mesdb);
    QSqlQuery ms_query_3(ms_mesdb);


    while(ms_query.next()){

        QString rework_name = ms_query.value("REWORK_NAME").toString();
        QString rework_lottype_txt = QString("SELECT REWORK_NAME,MATERIAL_ID,SUM(REWORK_CHIP_QTY) AS rework_qty,TX_COMMENT "
                                             "FROM [V_FAB_REWORK_LOTS] "
                                             "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                             "AND LOT_TYPE = 'A' "
                                             "AND MATERIAL_GROUP = 'CSP' "
                                             "AND REWORK_NAME = '%3' "
                                             "group by REWORK_NAME,MATERIAL_ID,TX_COMMENT "
                                             "order by rework_qty desc;")
                                             .arg(start_date_str).arg(end_date_str).arg(rework_name);


        ms_query_2.exec(rework_lottype_txt);
        emit sig_debug_output(rework_lottype_txt);
        while(ms_query_2.next()){
            QString rework_MATERIAL_ID = ms_query_2.value("MATERIAL_ID").toString();
            QString rework_REWORK_NAME = ms_query_2.value("REWORK_NAME").toString();
            QString TX_COMMENT = ms_query_2.value("TX_COMMENT").toString();
            TX_COMMENT = TX_COMMENT.replace("'","''");
            QString rework_content_txt = QString("SELECT * "
                                                 "FROM [V_FAB_REWORK_LOTS] "
                                                 "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                                 "AND LOT_TYPE = 'A' "
                                                 "AND MATERIAL_GROUP = 'CSP' "
                                                 "AND MATERIAL_ID = '%3' "
                                                 "AND TX_COMMENT = '%4' "
                                                 "AND REWORK_NAME = '%5';").arg(start_date_str).arg(end_date_str)
                                                                            .arg(rework_MATERIAL_ID)
                                                                            .arg(TX_COMMENT)
                                                                            .arg(rework_REWORK_NAME);
            int lot_count = 0;
            int chip_count = 0;
            ms_query_3.exec(rework_content_txt);
            emit sig_debug_output(rework_content_txt);
            rework_text_type rework_item;
            while(ms_query_3.next()){
                lot_count++;
                chip_count += ms_query_3.value("REWORK_CHIP_QTY").toInt();
            }
            ms_query_3.first();
            QString rework_lotid = ms_query_3.value("MOTHER_LOT_ID").toString();
            rework_item.setLOT(rework_lotid);
            rework_item.setProcess(ms_query_3.value("OPERATION_SHORT_NAME").toString());
            rework_item.setChip_count(chip_count);
            rework_item.setLot_count(lot_count);
            rework_item.setContent(ms_query_3.value("REWORK_NAME").toString());
            rework_item.setProduct_name(rework_MATERIAL_ID);
            rework_worstcpsprocesslist.append(rework_item);
        }
    }


    QStringList defect_worst_list;
    defect_worst_list<<tr("machinefail(probe)");
    defect_worst_list<<tr("machinefail(eatching)");
    defect_worst_list<<tr("machinefail(light)");
    defect_worst_list<<tr("machinefail(deposition)");
    defect_worst_list<<tr("workerfail(probe)");
    defect_worst_list<<tr("workerfail(eatching)");
    defect_worst_list<<tr("workerfail(light)");
    defect_worst_list<<tr("workerfail(deposition)");
    defect_worst_list<<tr("workerfail(output)");

    for(int i=0;i<defect_worst_list.count();i++){
        QString defect_lot_txt = QString("SELECT TX_COMMENT,MATERIAL_ID "
                                         "FROM [V_FAB_DEFECT_LOTS] "
                                         "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                         "AND LOT_TYPE = 'A' "
                                         "AND MATERIAL_GROUP = 'CSP' "
                                         "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                         "AND DEFECT_NAME = '%3' "
                                         "group by TX_COMMENT,MATERIAL_ID ").arg(start_date_str).arg(end_date_str)
                                                                 .arg(defect_worst_list.at(i));

            ms_query.exec(defect_lot_txt);
            emit sig_debug_output(defect_lot_txt);
            while(ms_query.next()){
                QString TX_COMMENT = ms_query.value("TX_COMMENT").toString();
                TX_COMMENT = TX_COMMENT.replace("'","''");
                QString MATERIAL_ID = ms_query.value("MATERIAL_ID").toString();
                QString defect_lot_wrost_txt = QString("SELECT * "
                                                 "FROM [V_FAB_DEFECT_LOTS] "
                                                 "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                                 "AND LOT_TYPE = 'A' "
                                                 "AND MATERIAL_GROUP = 'CSP' "
                                                 "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                                 "AND DEFECT_NAME = '%3' "
                                                 "AND TX_COMMENT = '%4' "
                                                 "AND MATERIAL_ID = '%5'").arg(start_date_str).arg(end_date_str)
                                                                         .arg(defect_worst_list.at(i)).arg(TX_COMMENT).arg(MATERIAL_ID);

            ms_query_2.exec(defect_lot_wrost_txt);
            emit sig_debug_output(defect_lot_wrost_txt);
//            qDebug()<<ms_query_2.lastQuery();
            int lot_count = 0;
            int chip_count = 0;
            while(ms_query_2.next()){
                lot_count++;
                chip_count += ms_query_2.value("DEFECT_QTY").toInt();
            }
            ms_query_2.first();
            defect_worst_type defect_item;
            QString defect_lotid = ms_query_2.value("LOT_ID").toString();
            defect_lotid = defect_lotid.mid(0,defect_lotid.length()-1);
            defect_lotid = defect_lotid + "0";
            defect_item.setLOT(defect_lotid);
            defect_item.setDefect_name(ms_query_2.value("DEFECT_NAME").toString());
            defect_item.setChip_count(chip_count);
            defect_item.setLot_count(lot_count);
            defect_item.setProduct_name(ms_query_2.value("MATERIAL_ID").toString());
            defect_item.setContent(TX_COMMENT);

            QString defect_materialgroup_txt = QString("SELECT TOP 1 * "
                                                   "FROM [V_SCM_LOTSUMMARY] "
                                                   "where MATERIAL_ID = '%1' AND WORK_DATE = '%2';")
                                                    .arg(defect_item.getProduct_name()).arg(select_date.toString("yyyyMMdd"));
            ms_query_3.exec(defect_materialgroup_txt);
            emit sig_debug_output(defect_materialgroup_txt);
            if(ms_query_3.next()){
                defect_item.setMetarial_grouptype(ms_query_3.value("MATERIAL_GROUP1").toString());
            }
            defect_worstcpsprocesslist.append(defect_item);
        }
    }

    for(int i=0;i<defect_worstcpsprocesslist.count();i++){
        defect_worst_type defect_item;
        defect_item = defect_worstcpsprocesslist.at(i);
//        qDebug()<<select_date.toString("MMdd")<<" product = "<<defect_item.getProduct_name()
//               <<" LOTNO = "<<defect_item.getLOT()<<" LOTCOUNT = "<<defect_item.getLot_count()
//               <<" process = "<<defect_item.getProcess()<<" chipcount = "<<defect_item.getChip_count()
//               <<" money code = "<<defect_item.getMetarial_grouptype()
//               <<" contect = "<<defect_item.getContent()
//               <<" DEFECT_NAME = "<<defect_item.getDefect_name();
    }
#else

#endif
#ifdef REAL_QUERY
    for(int i=0;i<EXTERIOR_DEFECT_ITEM_LIST.count();i++){
        QString exterior_txt = QString("SELECT SUM(DEFECT_QTY)DEFECT_SUM "
                                       "FROM [V_FAB_DEFECT_LOTS] "
                                       "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                       "AND LOT_TYPE = 'A' "
                                       "AND MATERIAL_GROUP = 'CSP' "
                                       "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                       "AND OPERATION_SHORT_NAME = '%3' "
                                       "AND REPROCESS_FLAG = 'N' "
                                       "AND DEFECT_NAME = '%4';").arg(start_date_str).arg(end_date_str).arg(tr("exterior")).arg(EXTERIOR_DEFECT_ITEM_LIST.at(i));
          ms_query.exec(exterior_txt);
          emit sig_debug_output(exterior_txt);
          if(ms_query.next()){
              int count = ms_query.value("DEFECT_SUM").toInt();
              double vaild_rate;
              double temp1 = 100.0-exterior_vaild;
              double temp2 = exterior_worst_count;
              if(count!=0){
                  vaild_rate = ((count/temp2)*temp1);

                   if(EXTERIOR_DEFECT_ITEM_LIST.at(i)==tr("padpaticle")){
                        exterior_padpaticle_vaild = vaild_rate;
                   }else if(EXTERIOR_DEFECT_ITEM_LIST.at(i)==tr("etcpaticle")){
                        exterior_etcpaticle_vaild = vaild_rate;
                   }else if(EXTERIOR_DEFECT_ITEM_LIST.at(i)==tr("pattenpaticle")){
                        exterior_pattenpaticle_vaild = vaild_rate;
                   }else if(EXTERIOR_DEFECT_ITEM_LIST.at(i)==tr("sin_diseatching_worst")){
                        exterior_sindiseatching_vaild = vaild_rate;
                   }else if(EXTERIOR_DEFECT_ITEM_LIST.at(i)==tr("brigit_pad_worst")){
                        exterior_brigitpadworst_vaild = vaild_rate;
                   }
              }
          }
    }
#else

#endif
#ifdef REAL_QUERY
    for(int i=0;i<CSP_LIMIT_PRODUCT_LIST.count();i++){
        QString CSP_limit_product_query = QString("SELECT SUM(INPUT_QTY)INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM "
                                                  "FROM [V_FAB_OUTPUT_LOTS] "
                                                  "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                                  "AND LOT_TYPE = 'B' "
                                                  "AND MATERIAL_GROUP = 'CSP' "
                                                  "AND MATERIAL_ID = '%3' "
                                                  "AND OPERATION_SHORT_NAME = '%4'; ")
                                                   .arg(start_date_str).arg(end_date_str).arg(CSP_LIMIT_PRODUCT_LIST.at(i))
                                                    .arg(tr("lastprobe_vaild"));
        ms_query.exec(CSP_limit_product_query);
        if(ms_query.next()){
            double input_sum = ms_query.value("INPUT_SUM").toDouble();
            if(input_sum == 0){
                continue;
            }
            double output_sum = ms_query.value("OUTPUT_SUM").toDouble();
            double vaild = (output_sum/input_sum)*100.0;
            vaild = RoundOff(vaild,2);
            csp_limit_product_type item;
            item.setProduct_name(CSP_LIMIT_PRODUCT_LIST.at(i));
            item.setInput_sum(input_sum);
            item.setOutput_sum(output_sum);
            item.setVaild(vaild);
            CSP_Limit_product_item_list.append(item);
        }
    }
#else

#endif
    emit sig_debug_output("complete go excel");
    emit sig_excel_work();

}

product_item::product_item(QString product_name)
{
    this->product_name = product_name;
    total_fail_count = 0;
    probe_DP001_count = 0;
    probe_DP001_rate = 0;
    probe_DP002_count = 0;
    probe_DP002_rate = 0;
    probe_DP003_count = 0;
    probe_DP003_rate = 0;
    probe_DP004_count = 0;
    probe_DP004_rate = 0;
    probe_DP005_count = 0;
    probe_DP005_rate = 0;
    probe_DP006_count = 0;
    probe_DP006_rate = 0;
    probe_DP008_count = 0;
    probe_DP008_rate = 0;

    workerfail_probe_count = 0;
    workerfail_probe_rate = 0;
    workerfail_eatching_count = 0;
    workerfail_eatching_rate = 0;
    workerfail_light_count = 0;
    workerfail_light_rate = 0;
    workerfail_deposition_count = 0;
    workerfail_deposition_rate = 0;
    workerfail_output_count = 0;
    workerfail_output_rate = 0;

    machinefail_probe_count = 0;
    machinefail_probe_rate = 0;
    machinefail_eatching_count = 0;
    machinefail_eatching_rate = 0;
    machinefail_light_count = 0;
    machinefail_light_rate = 0;
    machinefail_deposition_count = 0;
    machinefail_deposition_rate = 0;


    depositionpaticle_count=0;
    depositionpaticle_rate=0;
    padpaticle_count = 0;
    padpaticle_rate =0;
    etcpaticle_count =0;
    etcpaticle_rate =0;
    pattenpaticle_count =0;
    pattenpaticle_rate =0;
    jobmiss_probe_count =0;
    jobmiss_probe_rate =0;
    jobmiss_eatching_count =0;
    jobmiss_eatching_rate =0;
    jobmiss_light_count=0;
    jobmiss_light_rate=0;
    jobmiss_deposition_count=0;
    jobmiss_deposition_rate=0;
    rework_paticle_count=0;
    rework_paticle_rate =0;


    prcdhigh_count =0;
    prcdhigh_rate = 0;
    prcdlow_count = 0;
    prcdlow_rate = 0;

    metalhigh_count = 0;
    metalhigh_rate = 0;
    metallow_count = 0;
    metallow_rate = 0;




}

double product_item::getMetallow_rate() const
{
    return metallow_rate;
}

void product_item::setMetallow_rate(double value)
{
    metallow_rate = value;
}

int product_item::getMetallow_count() const
{
    return metallow_count;
}

void product_item::setMetallow_count(int value)
{
    metallow_count = value;
}

double product_item::getMetalhigh_rate() const
{
    return metalhigh_rate;
}

void product_item::setMetalhigh_rate(double value)
{
    metalhigh_rate = value;
}

int product_item::getMetalhigh_count() const
{
    return metalhigh_count;
}

void product_item::setMetalhigh_count(int value)
{
    metalhigh_count = value;
}

double product_item::getPrcdlow_rate() const
{
    return prcdlow_rate;
}

void product_item::setPrcdlow_rate(double value)
{
    prcdlow_rate = value;
}

int product_item::getPrcdlow_count() const
{
    return prcdlow_count;
}

void product_item::setPrcdlow_count(int value)
{
    prcdlow_count = value;
}

double product_item::getPrcdhigh_rate() const
{
    return prcdhigh_rate;
}

void product_item::setPrcdhigh_rate(double value)
{
    prcdhigh_rate = value;
}

int product_item::getPrcdhigh_count() const
{
    return prcdhigh_count;
}

void product_item::setPrcdhigh_count(int value)
{
    prcdhigh_count = value;
}

double product_item::getJobmiss_deposition_rate() const
{
    return jobmiss_deposition_rate;
}

void product_item::setJobmiss_deposition_rate(double value)
{
    jobmiss_deposition_rate = value;
}

int product_item::getJobmiss_deposition_count() const
{
    return jobmiss_deposition_count;
}

void product_item::setJobmiss_deposition_count(int value)
{
    jobmiss_deposition_count = value;
}

double product_item::getJobmiss_light_rate() const
{
    return jobmiss_light_rate;
}

void product_item::setJobmiss_light_rate(double value)
{
    jobmiss_light_rate = value;
}

int product_item::getJobmiss_light_count() const
{
    return jobmiss_light_count;
}

void product_item::setJobmiss_light_count(int value)
{
    jobmiss_light_count = value;
}

double product_item::getJobmiss_eatching_rate() const
{
    return jobmiss_eatching_rate;
}

void product_item::setJobmiss_eatching_rate(double value)
{
    jobmiss_eatching_rate = value;
}

int product_item::getJobmiss_eatching_count() const
{
    return jobmiss_eatching_count;
}

void product_item::setJobmiss_eatching_count(int value)
{
    jobmiss_eatching_count = value;
}

double product_item::getJobmiss_probe_rate() const
{
    return jobmiss_probe_rate;
}

void product_item::setJobmiss_probe_rate(double value)
{
    jobmiss_probe_rate = value;
}

int product_item::getJobmiss_probe_count() const
{
    return jobmiss_probe_count;
}

void product_item::setJobmiss_probe_count(int value)
{
    jobmiss_probe_count = value;
}

double product_item::getRework_paticle_rate() const
{
    return rework_paticle_rate;
}

void product_item::setRework_paticle_rate(double value)
{
    rework_paticle_rate = value;
}

int product_item::getRework_paticle_count() const
{
    return rework_paticle_count;
}

void product_item::setRework_paticle_count(int value)
{
    rework_paticle_count = value;
}

double product_item::getDepositionpaticle_rate() const
{
    return depositionpaticle_rate;
}

void product_item::setDepositionpaticle_rate(double value)
{
    depositionpaticle_rate = value;
}

int product_item::getDepositionpaticle_count() const
{
    return depositionpaticle_count;
}

void product_item::setDepositionpaticle_count(int value)
{
    depositionpaticle_count = value;
}

double product_item::getPattenpaticle_rate() const
{
    return pattenpaticle_rate;
}

void product_item::setPattenpaticle_rate(double value)
{
    pattenpaticle_rate = value;
}

int product_item::getPattenpaticle_count() const
{
    return pattenpaticle_count;
}

void product_item::setPattenpaticle_count(int value)
{
    pattenpaticle_count = value;
}

double product_item::getEtcpaticle_rate() const
{
    return etcpaticle_rate;
}

void product_item::setEtcpaticle_rate(double value)
{
    etcpaticle_rate = value;
}

int product_item::getEtcpaticle_count() const
{
    return etcpaticle_count;
}

void product_item::setEtcpaticle_count(int value)
{
    etcpaticle_count = value;
}

double product_item::getPadpaticle_rate() const
{
    return padpaticle_rate;
}

void product_item::setPadpaticle_rate(double value)
{
    padpaticle_rate = value;
}

int product_item::getPadpaticle_count() const
{
    return padpaticle_count;
}

void product_item::setPadpaticle_count(int value)
{
    padpaticle_count = value;
}

double product_item::getMachinefail_deposition_rate() const
{
    return machinefail_deposition_rate;
}

void product_item::setMachinefail_deposition_rate(double value)
{
    machinefail_deposition_rate = value;
}

int product_item::getMachinefail_deposition_count() const
{
    return machinefail_deposition_count;
}

void product_item::setMachinefail_deposition_count(int value)
{
    machinefail_deposition_count = value;
}

double product_item::getMachinefail_light_rate() const
{
    return machinefail_light_rate;
}

void product_item::setMachinefail_light_rate(double value)
{
    machinefail_light_rate = value;
}

int product_item::getMachinefail_light_count() const
{
    return machinefail_light_count;
}

void product_item::setMachinefail_light_count(int value)
{
    machinefail_light_count = value;
}

double product_item::getMachinefail_eatching_rate() const
{
    return machinefail_eatching_rate;
}

void product_item::setMachinefail_eatching_rate(double value)
{
    machinefail_eatching_rate = value;
}

int product_item::getMachinefail_eatching_count() const
{
    return machinefail_eatching_count;
}

void product_item::setMachinefail_eatching_count(int value)
{
    machinefail_eatching_count = value;
}

double product_item::getMachinefail_probe_rate() const
{
    return machinefail_probe_rate;
}

void product_item::setMachinefail_probe_rate(double value)
{
    machinefail_probe_rate = value;
}

int product_item::getMachinefail_probe_count() const
{
    return machinefail_probe_count;
}

void product_item::setMachinefail_probe_count(int value)
{
    machinefail_probe_count = value;
}

double product_item::getWorkerfail_output_rate() const
{
    return workerfail_output_rate;
}

void product_item::setWorkerfail_output_rate(double value)
{
    workerfail_output_rate = value;
}

int product_item::getWorkerfail_output_count() const
{
    return workerfail_output_count;
}

void product_item::setWorkerfail_output_count(int value)
{
    workerfail_output_count = value;
}

double product_item::getWorkerfail_deposition_rate() const
{
    return workerfail_deposition_rate;
}

void product_item::setWorkerfail_deposition_rate(double value)
{
    workerfail_deposition_rate = value;
}

int product_item::getWorkerfail_deposition_count() const
{
    return workerfail_deposition_count;
}

void product_item::setWorkerfail_deposition_count(int value)
{
    workerfail_deposition_count = value;
}

double product_item::getWorkerfail_light_rate() const
{
    return workerfail_light_rate;
}

void product_item::setWorkerfail_light_rate(double value)
{
    workerfail_light_rate = value;
}

int product_item::getWorkerfail_light_count() const
{
    return workerfail_light_count;
}

void product_item::setWorkerfail_light_count(int value)
{
    workerfail_light_count = value;
}

double product_item::getWorkerfail_eatching_rate() const
{
    return workerfail_eatching_rate;
}

void product_item::setWorkerfail_eatching_rate(double value)
{
    workerfail_eatching_rate = value;
}

int product_item::getWorkerfail_eatching_count() const
{
    return workerfail_eatching_count;
}

void product_item::setWorkerfail_eatching_count(int value)
{
    workerfail_eatching_count = value;
}

double product_item::getWorkerfail_probe_rate() const
{
    return workerfail_probe_rate;
}

void product_item::setWorkerfail_probe_rate(double value)
{
    workerfail_probe_rate = value;
}

int product_item::getWorkerfail_probe_count() const
{
    return workerfail_probe_count;
}

void product_item::setWorkerfail_probe_count(int value)
{
    workerfail_probe_count = value;
}

double product_item::getProbe_DP005_rate() const
{
    return probe_DP005_rate;
}

void product_item::setProbe_DP005_rate(double value)
{
    probe_DP005_rate = value;
}

int product_item::getProbe_DP005_count() const
{
    return probe_DP005_count;
}

void product_item::setProbe_DP005_count(int value)
{
    probe_DP005_count = value;
}

double product_item::getProbe_DP008_rate() const
{
    return probe_DP008_rate;
}

void product_item::setProbe_DP008_rate(double value)
{
    probe_DP008_rate = value;
}

int product_item::getProbe_DP008_count() const
{
    return probe_DP008_count;
}

void product_item::setProbe_DP008_count(int value)
{
    probe_DP008_count = value;
}

double product_item::getProbe_DP006_rate() const
{
    return probe_DP006_rate;
}

void product_item::setProbe_DP006_rate(double value)
{
    probe_DP006_rate = value;
}

int product_item::getProbe_DP006_count() const
{
    return probe_DP006_count;
}

void product_item::setProbe_DP006_count(int value)
{
    probe_DP006_count = value;
}

double product_item::getProbe_DP004_rate() const
{
    return probe_DP004_rate;
}

void product_item::setProbe_DP004_rate(double value)
{
    probe_DP004_rate = value;
}

int product_item::getProbe_DP004_count() const
{
    return probe_DP004_count;
}

void product_item::setProbe_DP004_count(int value)
{
    probe_DP004_count = value;
}

double product_item::getProbe_DP003_rate() const
{
    return probe_DP003_rate;
}

void product_item::setProbe_DP003_rate(double value)
{
    probe_DP003_rate = value;
}

int product_item::getProbe_DP003_count() const
{
    return probe_DP003_count;
}

void product_item::setProbe_DP003_count(int value)
{
    probe_DP003_count = value;
}

double product_item::getProbe_DP002_rate() const
{
    return probe_DP002_rate;
}

void product_item::setProbe_DP002_rate(double value)
{
    probe_DP002_rate = value;
}

int product_item::getProbe_DP002_count() const
{
    return probe_DP002_count;
}

void product_item::setProbe_DP002_count(int value)
{
    probe_DP002_count = value;
}

double product_item::getProbe_DP001_rate() const
{
    return probe_DP001_rate;
}

void product_item::setProbe_DP001_rate(double value)
{
    probe_DP001_rate = value;
}

int product_item::getProbe_DP001_count() const
{
    return probe_DP001_count;
}

void product_item::setProbe_DP001_count(int value)
{
    probe_DP001_count = value;
}

QString product_item::getProduct_name() const
{
    return product_name;
}

void product_item::setProduct_name(const QString &value)
{
    product_name = value;
}

int product_item::getTotal_fail_count() const
{
    return total_fail_count;
}

void product_item::setTotal_fail_count(int value)
{
    total_fail_count = value;
}

process_code::process_code()
{
    vield = 0;
    DEFECT_SUM = 0;
    EXCLUDE_YIELD_QTY_SUM =0;
}

double process_code::getEXCLUDE_YIELD_QTY_SUM() const
{
    return EXCLUDE_YIELD_QTY_SUM;
}

void process_code::setEXCLUDE_YIELD_QTY_SUM(double value)
{
    EXCLUDE_YIELD_QTY_SUM = value;
}

double process_code::getDEFECT_SUM() const
{
    return DEFECT_SUM;
}

void process_code::setDEFECT_SUM(double value)
{
    DEFECT_SUM = value;
}

double process_code::getVield() const
{
    return vield;
}

void process_code::setVield(double value)
{
    vield = value;
}

double process_code::getOUTPUT_SUM() const
{
    return OUTPUT_SUM;
}

void process_code::setOUTPUT_SUM(double value)
{
    OUTPUT_SUM = value;
}

double process_code::getINPUT_SUM() const
{
    return INPUT_SUM;
}

void process_code::setINPUT_SUM(double value)
{
    INPUT_SUM = value;
}

QString process_code::getName() const
{
    return name;
}

void process_code::setName(const QString &value)
{
    name = value;
}

QString process_code::getId() const
{
    return id;
}

void process_code::setId(const QString &value)
{
    id = value;
}

QString rework_text_type::getLOT() const
{
    return LOT;
}

void rework_text_type::setLOT(const QString &value)
{
    LOT = value;
}

QString rework_text_type::getProcess() const
{
    return process;
}

void rework_text_type::setProcess(const QString &value)
{
    process = value;
}

int rework_text_type::getLot_count() const
{
    return lot_count;
}

void rework_text_type::setLot_count(int value)
{
    lot_count = value;
}

int rework_text_type::getChip_count() const
{
    return chip_count;
}

void rework_text_type::setChip_count(int value)
{
    chip_count = value;
}

QString rework_text_type::getContent() const
{
    return content;
}

void rework_text_type::setContent(const QString &value)
{
    content = value;
}

QString rework_text_type::getProduct_name() const
{
    return product_name;
}

void rework_text_type::setProduct_name(const QString &value)
{
    product_name = value;
}

QString defect_worst_type::getLOT() const
{
    return LOT;
}

void defect_worst_type::setLOT(const QString &value)
{
    LOT = value;
}

QString defect_worst_type::getProcess() const
{
    return process;
}

void defect_worst_type::setProcess(const QString &value)
{
    process = value;
}

int defect_worst_type::getLot_count() const
{
    return lot_count;
}

void defect_worst_type::setLot_count(int value)
{
    lot_count = value;
}

int defect_worst_type::getChip_count() const
{
    return chip_count;
}

void defect_worst_type::setChip_count(int value)
{
    chip_count = value;
}

QString defect_worst_type::getDefect_name() const
{
    return Defect_name;
}

void defect_worst_type::setDefect_name(const QString &value)
{
    Defect_name = value;
}

QString defect_worst_type::getContent() const
{
    return content;
}

void defect_worst_type::setContent(const QString &value)
{
    content = value;
}
QString defect_worst_type::getMetarial_grouptype() const
{
    return metarial_grouptype;
}

void defect_worst_type::setMetarial_grouptype(const QString &value)
{
    metarial_grouptype = value;
}

QString defect_worst_type::getProduct_name() const
{
    return product_name;
}

void defect_worst_type::setProduct_name(const QString &value)
{
    product_name = value;
}

double csp_limit_product_type::getInput_sum() const
{
    return input_sum;
}

void csp_limit_product_type::setInput_sum(double value)
{
    input_sum = value;
}

double csp_limit_product_type::getOutput_sum() const
{
    return output_sum;
}

void csp_limit_product_type::setOutput_sum(double value)
{
    output_sum = value;
}

double csp_limit_product_type::getVaild() const
{
    return vaild;
}

void csp_limit_product_type::setVaild(double value)
{
    vaild = value;
}

QString csp_limit_product_type::getProduct_name() const
{
    return product_name;
}

void csp_limit_product_type::setProduct_name(const QString &value)
{
    product_name = value;
}
