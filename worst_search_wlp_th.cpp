#include "worst_search_wlp_th.h"

worst_search_WLP_th::worst_search_WLP_th()
{
    QString msdb_name = QString("MS_MESDB_WLP_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString mydb_name = QString("MY_MESDB_WLP_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
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

    PROBE_ITEM_LIST.append("DP001");//OS불량
    PROBE_ITEM_LIST.append("DP003");//저주파
    PROBE_ITEM_LIST.append("DP004");//고주파
    PROBE_ITEM_LIST.append("DP005");//BW 불량
    PROBE_ITEM_LIST.append("DP006");//LIMIT 불량
//    PROBE_ITEM_LIST.append("DP007");//DF 큼
    PROBE_ITEM_LIST.append("DP008");//VSWR불량
}

QDate worst_search_WLP_th::getSelect_date() const
{
    return select_date;
}

void worst_search_WLP_th::setSelect_date(const QDate &value)
{
    select_date = value;
}

double worst_search_WLP_th::getExterior_worst_count() const
{
    return exterior_worst_count;
}

void worst_search_WLP_th::setExterior_worst_count(double value)
{
    exterior_worst_count = value;
}

double worst_search_WLP_th::getExterior_vaild() const
{
    return exterior_vaild;
}

void worst_search_WLP_th::setExterior_vaild(double value)
{
    exterior_vaild = value;
}

double worst_search_WLP_th::getExterior_brigitpadworst_vaild() const
{
    return exterior_brigitpadworst_vaild;
}

void worst_search_WLP_th::setExterior_brigitpadworst_vaild(double value)
{
    exterior_brigitpadworst_vaild = value;
}

double worst_search_WLP_th::getExterior_etcpaticle_vaild() const
{
    return exterior_etcpaticle_vaild;
}

void worst_search_WLP_th::setExterior_etcpaticle_vaild(double value)
{
    exterior_etcpaticle_vaild = value;
}

double worst_search_WLP_th::getExterior_sindiseatching_vaild() const
{
    return exterior_sindiseatching_vaild;
}

void worst_search_WLP_th::setExterior_sindiseatching_vaild(double value)
{
    exterior_sindiseatching_vaild = value;
}

double worst_search_WLP_th::getExterior_padpaticle_vaild() const
{
    return exterior_padpaticle_vaild;
}

void worst_search_WLP_th::setExterior_padpaticle_vaild(double value)
{
    exterior_padpaticle_vaild = value;
}

double worst_search_WLP_th::getExterior_pattenpaticle_vaild() const
{
    return exterior_pattenpaticle_vaild;
}

void worst_search_WLP_th::setExterior_pattenpaticle_vaild(double value)
{
    exterior_pattenpaticle_vaild = value;
}

double worst_search_WLP_th::getTotal_Jobmiss_probe() const
{
    return total_Jobmiss_probe;
}

void worst_search_WLP_th::setTotal_Jobmiss_probe(double value)
{
    total_Jobmiss_probe = value;
}

double worst_search_WLP_th::getTotal_Jobmiss_light() const
{
    return total_Jobmiss_light;
}

void worst_search_WLP_th::setTotal_Jobmiss_light(double value)
{
    total_Jobmiss_light = value;
}

double worst_search_WLP_th::getTotal_Jobmiss_eatching() const
{
    return total_Jobmiss_eatching;
}

void worst_search_WLP_th::setTotal_Jobmiss_eatching(double value)
{
    total_Jobmiss_eatching = value;
}

double worst_search_WLP_th::getTotal_Jobmiss_defect() const
{
    return total_Jobmiss_defect;
}

void worst_search_WLP_th::setTotal_Jobmiss_defect(double value)
{
    total_Jobmiss_defect = value;
}

double worst_search_WLP_th::getTotal_Rework_paticle() const
{
    return total_Rework_paticle;
}

void worst_search_WLP_th::setTotal_Rework_paticle(double value)
{
    total_Rework_paticle = value;
}

double worst_search_WLP_th::getTotal_brigit_pad_worst() const
{
    return total_brigit_pad_worst;
}

void worst_search_WLP_th::setTotal_brigit_pad_worst(double value)
{
    total_brigit_pad_worst = value;
}

double worst_search_WLP_th::getTotal_Defectpaticle() const
{
    return total_Defectpaticle;
}

void worst_search_WLP_th::setTotal_Defectpaticle(double value)
{
    total_Defectpaticle = value;
}

double worst_search_WLP_th::getTotal_Pattenpaticle() const
{
    return total_Pattenpaticle;
}

void worst_search_WLP_th::setTotal_Pattenpaticle(double value)
{
    total_Pattenpaticle = value;
}

double worst_search_WLP_th::getTotal_Etcpaticle() const
{
    return total_Etcpaticle;
}

void worst_search_WLP_th::setTotal_Etcpaticle(double value)
{
    total_Etcpaticle = value;
}

double worst_search_WLP_th::getTotal_Padpaticle() const
{
    return total_Padpaticle;
}

void worst_search_WLP_th::setTotal_Padpaticle(double value)
{
    total_Padpaticle = value;
}

double worst_search_WLP_th::getTotal_Machinefail_defect() const
{
    return total_Machinefail_defect;
}

void worst_search_WLP_th::setTotal_Machinefail_defect(double value)
{
    total_Machinefail_defect = value;
}

double worst_search_WLP_th::getTotal_Machinefail_light() const
{
    return total_Machinefail_light;
}

void worst_search_WLP_th::setTotal_Machinefail_light(double value)
{
    total_Machinefail_light = value;
}

double worst_search_WLP_th::getTotal_Machinefail_eatching() const
{
    return total_Machinefail_eatching;
}

void worst_search_WLP_th::setTotal_Machinefail_eatching(double value)
{
    total_Machinefail_eatching = value;
}

double worst_search_WLP_th::getTotal_Machinefail_probe() const
{
    return total_Machinefail_probe;
}

void worst_search_WLP_th::setTotal_Machinefail_probe(double value)
{
    total_Machinefail_probe = value;
}

double worst_search_WLP_th::getTotal_Workerfail_output() const
{
    return total_Workerfail_output;
}

void worst_search_WLP_th::setTotal_Workerfail_output(double value)
{
    total_Workerfail_output = value;
}

double worst_search_WLP_th::getTotal_Workerfail_defect() const
{
    return total_Workerfail_defect;
}

void worst_search_WLP_th::setTotal_Workerfail_defect(double value)
{
    total_Workerfail_defect = value;
}

double worst_search_WLP_th::getTotal_Workerfail_light() const
{
    return total_Workerfail_light;
}

void worst_search_WLP_th::setTotal_Workerfail_light(double value)
{
    total_Workerfail_light = value;
}

double worst_search_WLP_th::getTotal_Workerfail_eatching() const
{
    return total_Workerfail_eatching;
}

void worst_search_WLP_th::setTotal_Workerfail_eatching(double value)
{
    total_Workerfail_eatching = value;
}

double worst_search_WLP_th::getTotal_Workerfail_probe() const
{
    return total_Workerfail_probe;
}

void worst_search_WLP_th::setTotal_Workerfail_probe(double value)
{
    total_Workerfail_probe = value;
}

double worst_search_WLP_th::getTotal_DP008_worst() const
{
    return total_DP008_worst;
}

void worst_search_WLP_th::setTotal_DP008_worst(double value)
{
    total_DP008_worst = value;
}

double worst_search_WLP_th::getTotal_DP006_worst() const
{
    return total_DP006_worst;
}

void worst_search_WLP_th::setTotal_DP006_worst(double value)
{
    total_DP006_worst = value;
}

double worst_search_WLP_th::getTotal_DP005_worst() const
{
    return total_DP005_worst;
}

void worst_search_WLP_th::setTotal_DP005_worst(double value)
{
    total_DP005_worst = value;
}

double worst_search_WLP_th::getTotal_DP004_worst() const
{
    return total_DP004_worst;
}

void worst_search_WLP_th::setTotal_DP004_worst(double value)
{
    total_DP004_worst = value;
}

double worst_search_WLP_th::getTotal_DP003_worst() const
{
    return total_DP003_worst;
}

void worst_search_WLP_th::setTotal_DP003_worst(double value)
{
    total_DP003_worst = value;
}

double worst_search_WLP_th::getTotal_DP001_worst() const
{
    return total_DP001_worst;
}

void worst_search_WLP_th::setTotal_DP001_worst(double value)
{
    total_DP001_worst = value;
}

double worst_search_WLP_th::getWorst_sum() const
{
    return worst_sum;
}

void worst_search_WLP_th::setWorst_sum(double value)
{
    worst_sum = value;
}

double worst_search_WLP_th::getLast_probe_vild_daily() const
{
    return last_probe_vild_daily;
}

void worst_search_WLP_th::setLast_probe_vild_daily(double value)
{
    last_probe_vild_daily = value;
}

double worst_search_WLP_th::getAccumulate_totalvild() const
{
    return accumulate_totalvild;
}

void worst_search_WLP_th::setAccumulate_totalvild(double value)
{
    accumulate_totalvild = value;
}

double worst_search_WLP_th::getDaily_totalvild() const
{
    return daily_totalvild;
}

void worst_search_WLP_th::setDaily_totalvild(double value)
{
    daily_totalvild = value;
}

void worst_search_WLP_th::run()
{
    daily_worstcpsprocesslist.clear();
    accumulate_worstcpsprocesslist.clear();
    rework_worstcpsprocesslist.clear();
    defect_worstcpsprocesslist.clear();

    QSqlQuery my_query(my_mesdb);
    QSqlQuery ms_query(ms_mesdb);
    QString start_date_str = select_date.toString("yyyyMMdd")+"080000";
    QString end_date_str = select_date.addDays(1).toString("yyyyMMdd")+"075959";
#ifdef REAL_QUERY_WLP
    my_query.exec("select * from MES_Worst_WLP_Process");
    while(my_query.next()){
        process_code_WLP data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());
        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'WLP\' "
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

#endif
    QDate firstday(select_date.year(),select_date.month(),1);
    QString accumulate_start_date_str = firstday.toString("yyyyMMdd")+"080000";

#ifdef REAL_QUERY_WLP
    my_query.exec("select * from MES_Worst_WLP_Process");
    while(my_query.next()){
        process_code_WLP data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());
        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'WLP\' "
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

#endif

#ifdef REAL_QUERY_WLP
    QString DEFECT_QTY_SUM_query = QString("SELECT SUM(DEFECT_QTY) DEFECT_SUM "
                                           "FROM [V_DEFECT_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP' "
                                           "AND EXCLUDE_YIELD_FLAG <> 'Y';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(DEFECT_QTY_SUM_query);
    ms_query.exec(DEFECT_QTY_SUM_query);

    ms_query.next();
    double defect_sum = ms_query.value("DEFECT_SUM").toDouble();

    QString REWORK_SUM_query = QString("SELECT SUM(REWORK_QTY) REWORK_SUM "
                                           "FROM [V_REWORK_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(REWORK_SUM_query);
    ms_query.exec(REWORK_SUM_query);
    ms_query.next();
    double rework_sum = ms_query.value("REWORK_SUM").toDouble();

    QString PROBE_SUM_query = QString("SELECT SUM(PROBE_INSP_QTY) PROBE_INSP_SUM "
                                           "FROM [V_PROBE_INSP_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP';")
                                           .arg(start_date_str).arg(end_date_str);
    emit sig_debug_output(PROBE_SUM_query);
    ms_query.exec(PROBE_SUM_query);
    ms_query.next();
    double probe_sum = ms_query.value("PROBE_INSP_SUM").toDouble();

    worst_sum = defect_sum+rework_sum+probe_sum;
#else

#endif
    emit sig_debug_output(QString("worst_sum = %1").arg(worst_sum));

#ifdef REAL_QUERY_WLP
    total_DP001_worst=0;
    total_DP003_worst=0;
    total_DP004_worst=0;
    total_DP005_worst=0;
    total_DP006_worst=0;
    total_DP008_worst=0;
    for(int j=0;j<PROBE_ITEM_LIST.count();j++){
        QString worstcount_query = QString("SELECT SUM(PROBE_INSP_QTY)PROBE_INSP_SUM "
                                           "FROM [V_PROBE_INSP_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP' "
                                           "AND PROBE_INSP_CODE = '%3';").arg(start_date_str).arg(end_date_str)
                                           .arg(PROBE_ITEM_LIST.at(j));

        ms_query.exec(worstcount_query);
        emit sig_debug_output(worstcount_query);
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
#endif
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

#ifdef REAL_QUERY_WLP
    for(int j=0;j<DEFECT_ITEM_LIST.count();j++){
        QString worstcount_query = QString("SELECT SUM(DEFECT_QTY)DEFECT_SUM "
                                           "FROM [V_DEFECT_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP' "
                                           "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                           "AND DEFECT_NAME = '%4'")
                                            .arg(start_date_str).arg(end_date_str)
                                            .arg(DEFECT_ITEM_LIST.at(j));

        ms_query.exec(worstcount_query);
        emit sig_debug_output(worstcount_query);
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
                }else if(DEFECT_ITEM_LIST.at(j)==tr("brigit_pad_worst")){//브릿지 패드
                    total_brigit_pad_worst = vaild_rate;
                }
            }
        }
    }
#else
#endif

    total_Rework_paticle=0;
    total_Jobmiss_defect=0;
    total_Jobmiss_eatching=0;
    total_Jobmiss_light=0;
    total_Jobmiss_probe=0;

#ifdef REAL_QUERY_WLP
    for(int j=0;j<REWORK_ITEM_LIST.count();j++){
        QString worstcount_query = QString("SELECT  SUM(REWORK_QTY)REWORK_SUM "
                                           "FROM [V_REWORK_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'WLP' "
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
                }
            }
        }
    }
#else
#endif

#ifdef REAL_QUERY_WLP
    QString rework_type_txt = QString("SELECT OPERATION_SHORT_NAME "
                                      "FROM [V_REWORK_LOTS] "
                                      "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                      "AND LOT_TYPE = 'A' "
                                      "AND MATERIAL_GROUP = 'WLP' "
                                      "group by OPERATION_SHORT_NAME;").arg(start_date_str).arg(end_date_str);

    ms_query.exec(rework_type_txt);
    QSqlQuery ms_query_2(ms_mesdb);
    QSqlQuery ms_query_3(ms_mesdb);
    while(ms_query.next()){
        QString rework_type = ms_query.value("OPERATION_SHORT_NAME").toString();
        QString rework_lottype_txt = QString("SELECT MATERIAL_ID "
                                          "FROM [V_REWORK_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                          "AND LOT_TYPE = 'A' "
                                          "AND MATERIAL_GROUP = 'WLP' "
                                          "AND OPERATION_SHORT_NAME = '%3' "
                                          "group by MATERIAL_ID;").arg(start_date_str).arg(end_date_str).arg(rework_type);
        ms_query_2.exec(rework_lottype_txt);
        emit sig_debug_output(rework_lottype_txt);
        while(ms_query_2.next()){
            QString rework_MATERIAL_ID = ms_query_2.value("MATERIAL_ID").toString();
            QString rework_content_txt = QString("SELECT * "
                                                 "FROM [V_REWORK_LOTS] "
                                                 "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                                 "AND LOT_TYPE = 'A' "
                                                 "AND MATERIAL_GROUP = 'WLP' "
                                                 "AND OPERATION_SHORT_NAME = '%3' "
                                                 "AND MATERIAL_ID = '%4';").arg(start_date_str).arg(end_date_str)
                                                                            .arg(rework_type).arg(rework_MATERIAL_ID);
            ms_query_3.exec(rework_content_txt);
            emit sig_debug_output(rework_content_txt);
            int lot_count = 0;
            int chip_count = 0;
            rework_text_type_WLP rework_item;
            while(ms_query_3.next()){
                lot_count++;
                chip_count += ms_query_3.value("REWORK_QTY").toInt();
            }
            ms_query_3.first();
            QString rework_lotid = ms_query_3.value("LOT_ID").toString();
            rework_lotid = rework_lotid.mid(0,rework_lotid.length()-1);
            rework_lotid = rework_lotid + "0";
            QString rework_worst_type = rework_type;
            rework_item.setLOT(rework_lotid);
            rework_item.setProcess(rework_type);
            rework_item.setChip_count(chip_count);
            rework_item.setLot_count(lot_count);
            rework_item.setContent(rework_worst_type);
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
    QString defect_lot_txt = QString("SELECT TX_COMMENT "
                                     "FROM [V_DEFECT_LOTS] "
                                     "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                     "AND LOT_TYPE = 'A' "
                                     "AND MATERIAL_GROUP = 'WLP' "
                                     "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                     "AND DEFECT_NAME = '%3' "
                                     "group by TX_COMMENT ").arg(start_date_str).arg(end_date_str)
                                                             .arg(defect_worst_list.at(i));

        ms_query.exec(defect_lot_txt);
        emit sig_debug_output(defect_lot_txt);
        while(ms_query.next()){
            QString TX_COMMENT = ms_query.value("TX_COMMENT").toString();
            QString defect_lot_wrost_txt = QString("SELECT * "
                                             "FROM [V_DEFECT_LOTS] "
                                             "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                             "AND LOT_TYPE = 'A' "
                                             "AND MATERIAL_GROUP = 'WLP' "
                                             "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                             "AND DEFECT_NAME = '%3' "
                                             "AND TX_COMMENT = '%4'").arg(start_date_str).arg(end_date_str)
                                                                     .arg(defect_worst_list.at(i)).arg(TX_COMMENT);

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
            defect_worst_type_WLP defect_item;
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
#else
#endif

#ifdef REAL_QUERY_WLP
    for(int i=0;i<EXTERIOR_DEFECT_ITEM_LIST.count();i++){
        QString exterior_txt = QString("SELECT SUM(DEFECT_QTY)DEFECT_SUM "
                                       "FROM [V_DEFECT_LOTS] "
                                       "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                       "AND LOT_TYPE = 'A' "
                                       "AND MATERIAL_GROUP = 'WLP' "
                                       "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                       "AND OPERATION_SHORT_NAME = '%3' "
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
    emit sig_debug_output("complete go excel");
    emit sig_excel_work();
}

QString process_code_WLP::getName() const
{
    return name;
}

void process_code_WLP::setName(const QString &value)
{
    name = value;
}

double process_code_WLP::getINPUT_SUM() const
{
    return INPUT_SUM;
}

void process_code_WLP::setINPUT_SUM(double value)
{
    INPUT_SUM = value;
}

double process_code_WLP::getOUTPUT_SUM() const
{
    return OUTPUT_SUM;
}

void process_code_WLP::setOUTPUT_SUM(double value)
{
    OUTPUT_SUM = value;
}

double process_code_WLP::getDEFECT_SUM() const
{
    return DEFECT_SUM;
}

void process_code_WLP::setDEFECT_SUM(double value)
{
    DEFECT_SUM = value;
}

double process_code_WLP::getEXCLUDE_YIELD_QTY_SUM() const
{
    return EXCLUDE_YIELD_QTY_SUM;
}

void process_code_WLP::setEXCLUDE_YIELD_QTY_SUM(double value)
{
    EXCLUDE_YIELD_QTY_SUM = value;
}

double process_code_WLP::getVield() const
{
    return vield;
}

void process_code_WLP::setVield(double value)
{
    vield = value;
}

process_code_WLP::process_code_WLP()
{
    vield=0;
    DEFECT_SUM = 0;
    EXCLUDE_YIELD_QTY_SUM =0;
}

QString process_code_WLP::getId() const
{
    return id;
}

void process_code_WLP::setId(const QString &value)
{
    id = value;
}


QString rework_text_type_WLP::getLOT() const
{
    return LOT;
}

void rework_text_type_WLP::setLOT(const QString &value)
{
    LOT = value;
}

QString rework_text_type_WLP::getProcess() const
{
    return process;
}

void rework_text_type_WLP::setProcess(const QString &value)
{
    process = value;
}

int rework_text_type_WLP::getLot_count() const
{
    return lot_count;
}

void rework_text_type_WLP::setLot_count(int value)
{
    lot_count = value;
}

int rework_text_type_WLP::getChip_count() const
{
    return chip_count;
}

void rework_text_type_WLP::setChip_count(int value)
{
    chip_count = value;
}

QString rework_text_type_WLP::getContent() const
{
    return content;
}

void rework_text_type_WLP::setContent(const QString &value)
{
    content = value;
}

QString rework_text_type_WLP::getProduct_name() const
{
    return product_name;
}

void rework_text_type_WLP::setProduct_name(const QString &value)
{
    product_name = value;
}

QString defect_worst_type_WLP::getLOT() const
{
    return LOT;
}

void defect_worst_type_WLP::setLOT(const QString &value)
{
    LOT = value;
}

QString defect_worst_type_WLP::getProcess() const
{
    return process;
}

void defect_worst_type_WLP::setProcess(const QString &value)
{
    process = value;
}

int defect_worst_type_WLP::getLot_count() const
{
    return lot_count;
}

void defect_worst_type_WLP::setLot_count(int value)
{
    lot_count = value;
}

int defect_worst_type_WLP::getChip_count() const
{
    return chip_count;
}

void defect_worst_type_WLP::setChip_count(int value)
{
    chip_count = value;
}

QString defect_worst_type_WLP::getDefect_name() const
{
    return Defect_name;
}

void defect_worst_type_WLP::setDefect_name(const QString &value)
{
    Defect_name = value;
}

QString defect_worst_type_WLP::getContent() const
{
    return content;
}

void defect_worst_type_WLP::setContent(const QString &value)
{
    content = value;
}

QString defect_worst_type_WLP::getMetarial_grouptype() const
{
    return metarial_grouptype;
}

void defect_worst_type_WLP::setMetarial_grouptype(const QString &value)
{
    metarial_grouptype = value;
}

QString defect_worst_type_WLP::getProduct_name() const
{
    return product_name;
}

void defect_worst_type_WLP::setProduct_name(const QString &value)
{
    product_name = value;
}
