#include "worst_search_excel_main.h"
#include "ui_worst_search_excel_main.h"

worst_search_excel_main::worst_search_excel_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::worst_search_excel_main)
{
    ui->setupUi(this);
    ms_mesdb = QSqlDatabase::addDatabase("QODBC","MS_MESDB1");
    QString serverinfo = "DRIVER={SQL Server};Server=10.20.10.221;Database=MESDB;Uid=fabview;Port=1433;Pwd=fabview";
    ms_mesdb.setDatabaseName(serverinfo);
    QString configini_str = qApp->applicationDirPath()+"/server.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    QString server_ip = settings.value("serverip").toString();
    QString db_port = settings.value("dbport").toString();
    QString ftp_port = settings.value("ftpport").toString();
    settings.endGroup();

    if(!my_mesdb.contains("MY_MESDB1")){
       my_mesdb = QSqlDatabase::addDatabase("QMYSQL","MY_MESDB1");
       my_mesdb.setHostName(server_ip);
       my_mesdb.setPort(db_port.toInt());
       my_mesdb.setUserName(DBID);
       my_mesdb.setPassword(DBPW);
       my_mesdb.setDatabaseName("FAB");
    }else {
       my_mesdb = QSqlDatabase::database("EISDB");
    }

    if(!my_mesdb.isOpen()){
        if(!my_mesdb.open()){
             int ret = QMessageBox::warning(this, tr("conntion false"),
                                               "server connection fail\n"
                                                  ""+my_mesdb.lastError().text(),
                                                    QMessageBox::Close);
        }
    }


    if(!ms_mesdb.open()){
        qDebug()<<"fasle";
        qDebug()<<ms_mesdb.lastError().text();
    }else {
        qDebug()<<"open";
    }
    ui->select_data->setDate(QDate::currentDate().addDays(-1));

    DEFECT_ITEM_LIST.append(tr("workerfail(probe)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(eatching)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(light)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(defect)"));
    DEFECT_ITEM_LIST.append(tr("workerfail(output)"));

    DEFECT_ITEM_LIST.append(tr("machinefail(probe)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(eatching)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(light)"));
    DEFECT_ITEM_LIST.append(tr("machinefail(defect)"));

    DEFECT_ITEM_LIST.append(tr("paticle"));
    DEFECT_ITEM_LIST.append(tr("padpaticle"));
    DEFECT_ITEM_LIST.append(tr("etcpaticle"));
    DEFECT_ITEM_LIST.append(tr("pattenpaticle"));

    REWORK_ITEM_LIST.append(tr("jobmiss(probe)"));//작업 미스(프로브)
    REWORK_ITEM_LIST.append(tr("jobmiss(eatching)"));//작업 미스(성막)
    REWORK_ITEM_LIST.append(tr("jobmiss(light)"));//작업 미스(노광)
    REWORK_ITEM_LIST.append(tr("jobmiss(defect)"));//작업 미스(성막)
    REWORK_ITEM_LIST.append(tr("paticle"));//이물

    PROBE_ITEM_LIST.append("DP001");//OS불량
    PROBE_ITEM_LIST.append("DP002");//IL불량
    PROBE_ITEM_LIST.append("DP003");//저주파
    PROBE_ITEM_LIST.append("DP004");//고주파
    PROBE_ITEM_LIST.append("DP005");//BW 불량
    PROBE_ITEM_LIST.append("DP006");//LIMIT 불량
//    PROBE_ITEM_LIST.append("DP007");//DF 큼
    PROBE_ITEM_LIST.append("DP008");//VSWR불량





}

worst_search_excel_main::~worst_search_excel_main()
{
    delete ui;
}

void worst_search_excel_main::on_start_btn_clicked()
{
    daily_worstcpsprocesslist.clear();
    QSqlQuery my_query(my_mesdb);
    QSqlQuery ms_query(ms_mesdb);
    QString start_date_str = ui->select_data->date().toString("yyyyMMdd")+"080000";
    QString end_date_str = ui->select_data->date().addDays(1).toString("yyyyMMdd")+"075959";

    my_query.exec("select * from MES_Worst_CPS_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN \'%1\' AND \'%2\' "
                                          "AND LOT_TYPE = \'A\' "
                                          "AND MATERIAL_GROUP = \'CSP\' "
                                          "AND OPERATION_SHORT_NAME = \'%3\';")
                                          .arg(start_date_str)
                                          .arg(end_date_str)
                                          .arg(data.getName());
        ms_query.exec(INPUT_OUTPUT_SUM_query);
        ms_query.next();
        data.setINPUT_SUM(ms_query.value("INPUT_SUM").toDouble());
        data.setOUTPUT_SUM(ms_query.value("OUTPUT_SUM").toDouble());
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());

        daily_worstcpsprocesslist.append(data);
    }
    double daily_totalvild = 1.0;
    for(int i=0;i<daily_worstcpsprocesslist.count();i++){
        if(daily_worstcpsprocesslist.at(i).getVield() != 0){
            daily_totalvild *= daily_worstcpsprocesslist.at(i).getVield();
        }
    }
    daily_totalvild *=100.0;
    daily_totalvild = roundf(daily_totalvild * 100) / 100;
    qDebug()<<"daily_total vaild = "<<daily_totalvild;
 // 시간적 이유로 디버그시 잠시 주석 처리
    my_query.exec("select * from MES_Worst_CPS_Process");
    while(my_query.next()){
        process_code data;
        data.setId(my_query.value("OPERATION_ID").toString());
        data.setName(my_query.value("OPERATION_SHORT_NAME").toString());
        QDate firstday(ui->select_data->date().year(),ui->select_data->date().month(),1);
        QString accumulate_start_date_str = firstday.toString("yyyyMMdd")+"080000";

        QString INPUT_OUTPUT_SUM_query = QString("SELECT SUM(INPUT_QTY) INPUT_SUM,SUM(OUTPUT_QTY)OUTPUT_SUM,SUM(DEFECT_QTY)DEFECT_SUM,SUM(EXCLUDE_YIELD_QTY) EXCLUDE_YIELD_SUM "
                                          "FROM [V_OUTPUT_LOTS] "
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
        data.setDEFECT_SUM(ms_query.value("DEFECT_SUM").toDouble());
        data.setEXCLUDE_YIELD_QTY_SUM(ms_query.value("EXCLUDE_YIELD_SUM").toDouble());
        data.setVield((data.getOUTPUT_SUM()+data.getEXCLUDE_YIELD_QTY_SUM())/data.getINPUT_SUM());

        accumulate_worstcpsprocesslist.append(data);
    }
    double accumulate_totalvild = 1.0;
    for(int i=0;i<accumulate_worstcpsprocesslist.count();i++){
        if(accumulate_worstcpsprocesslist.at(i).getVield() != 0){
            accumulate_totalvild *= accumulate_worstcpsprocesslist.at(i).getVield();
        }
    }
    accumulate_totalvild *=100.0;
    accumulate_totalvild = roundf(accumulate_totalvild * 100) / 100;
    qDebug()<<"accumulate_total vaild = "<<accumulate_totalvild;




    QString DEFECT_QTY_SUM_query = QString("SELECT SUM(DEFECT_QTY) DEFECT_SUM "
                                           "FROM [V_DEFECT_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP' "
                                           "AND EXCLUDE_YIELD_FLAG <> 'Y';")
                                           .arg(start_date_str).arg(end_date_str);

    ms_query.exec(DEFECT_QTY_SUM_query);
    ms_query.next();
    double defect_sum = ms_query.value("DEFECT_SUM").toDouble();

    QString REWORK_SUM_query = QString("SELECT SUM(REWORK_QTY) REWORK_SUM "
                                           "FROM [V_REWORK_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str);

    ms_query.exec(REWORK_SUM_query);
    ms_query.next();
    double rework_sum = ms_query.value("REWORK_SUM").toDouble();

    QString PROBE_SUM_query = QString("SELECT SUM(PROBE_INSP_QTY) PROBE_INSP_SUM "
                                           "FROM [V_PROBE_INSP_LOTS] "
                                           "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                           "AND LOT_TYPE = 'A' "
                                           "AND MATERIAL_GROUP = 'CSP';")
                                           .arg(start_date_str).arg(end_date_str);

    ms_query.exec(PROBE_SUM_query);
    ms_query.next();
    double probe_sum = ms_query.value("PROBE_INSP_SUM").toDouble();

    double worst_sum = defect_sum+rework_sum+probe_sum;
    qDebug()<<"worst_sum = "<<worst_sum;



    QString worstitemtype_query = QString("SELECT MATERIAL_ID FROM [V_OUTPUT_LOTS] "
                                          "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                          "AND MATERIAL_GROUP = 'CSP' "
                                          "AND LOT_TYPE = 'A' "
                                          "AND INPUT_QTY !=OUTPUT_QTY "
                                          "group by MATERIAL_ID;").arg(start_date_str).arg(end_date_str);
    ms_query.exec(worstitemtype_query);
    while(ms_query.next()){
        QString product_lotname = ms_query.value("MATERIAL_ID").toString();
        product_item *worst_item = new product_item(product_lotname);
        item_vector.append(worst_item);
    }

    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        for(int j=0;j<PROBE_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT SUM(PROBE_INSP_QTY)PROBE_INSP_SUM "
                                               "FROM [V_PROBE_INSP_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND MATERIAL_ID = '%3' "
                                               "AND PROBE_INSP_CODE = '%4';").arg(start_date_str).arg(end_date_str)
                                                .arg(item_vector.at(i)->getProduct_name()).arg(PROBE_ITEM_LIST.at(j));

            ms_query.exec(worstcount_query);

            if(ms_query.next()){
                int count = ms_query.value("PROBE_INSP_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                vaild_rate = ((count/temp2)*temp1);

                if(PROBE_ITEM_LIST.at(j)=="DP001"){     //OS 불량 .
                    worst_item->setProbe_DP001_count(count);
                    worst_item->setProbe_DP001_rate(vaild_rate);
                }else if(PROBE_ITEM_LIST.at(j)=="DP003"){  //저주파 .
                    worst_item->setProbe_DP003_count(count);
                    worst_item->setProbe_DP003_rate(vaild_rate);
                }else if(PROBE_ITEM_LIST.at(j)=="DP004"){  //고주파 .
                    worst_item->setProbe_DP004_count(count);
                    worst_item->setProbe_DP004_rate(vaild_rate);
                }else if(PROBE_ITEM_LIST.at(j)=="DP005"){  //BW 불량 .
                    worst_item->setProbe_DP005_count(count);
                    worst_item->setProbe_DP005_rate(vaild_rate);
                }else if(PROBE_ITEM_LIST.at(j)=="DP006"){ //LIMIT 불량
                    worst_item->setProbe_DP006_count(count);
                    worst_item->setProbe_DP006_rate(vaild_rate);
                }else if(PROBE_ITEM_LIST.at(j)=="DP008"){  //VSWR불량
                    worst_item->setProbe_DP008_count(count);
                    worst_item->setProbe_DP008_rate(vaild_rate);
                }
            }
        }
    }
    double total_DP001_worst=0;
    double total_DP003_worst=0;
    double total_DP004_worst=0;
    double total_DP005_worst=0;
    double total_DP006_worst=0;
    double total_DP008_worst=0;
    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        total_DP001_worst += worst_item->getProbe_DP001_rate();
        total_DP003_worst += worst_item->getProbe_DP003_rate();
        total_DP004_worst += worst_item->getProbe_DP004_rate();
        total_DP005_worst += worst_item->getProbe_DP005_rate();
        total_DP006_worst += worst_item->getProbe_DP006_rate();
        total_DP008_worst += worst_item->getProbe_DP008_rate();

    }
    qDebug()<<"p OS DP001 = "<<RoundOff(total_DP001_worst,2);
    qDebug()<<"p lowfre DP003 = "<<RoundOff(total_DP003_worst,2);
    qDebug()<<"p highfre DP004 = "<<RoundOff(total_DP004_worst,2);
    qDebug()<<"p bw DP005 = "<<RoundOff(total_DP005_worst,2);
    qDebug()<<"p limit DP006 = "<<RoundOff(total_DP006_worst,2);
    qDebug()<<"p vswr DP008 = "<<RoundOff(total_DP008_worst,2);

    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        for(int j=0;j<DEFECT_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT SUM(DEFECT_QTY)DEFECT_SUM "
                                               "FROM [V_DEFECT_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND EXCLUDE_YIELD_FLAG <> 'Y' "
                                               "AND MATERIAL_ID = '%3' "
                                               "AND DEFECT_NAME = '%4'")
                                                .arg(start_date_str).arg(end_date_str)
                                                .arg(item_vector.at(i)->getProduct_name()).arg(DEFECT_ITEM_LIST.at(j));

            ms_query.exec(worstcount_query);

            if(ms_query.next()){
                int count = ms_query.value("DEFECT_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                vaild_rate = ((count/temp2)*temp1);

                if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(probe)")){     //작업자 파손(프로브)
                    worst_item->setWorkerfail_probe_count(count);
                    worst_item->setWorkerfail_probe_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(eatching)")){  //작업자 파손(에칭)
                    worst_item->setWorkerfail_eatching_count(count);
                    worst_item->setWorkerfail_eatching_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(light)")){  //작업자 파손(노광)
                    worst_item->setWorkerfail_light_count(count);
                    worst_item->setWorkerfail_light_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(defect)")){  //작업자 파손(성막)
                    worst_item->setWorkerfail_defect_count(count);
                    worst_item->setWorkerfail_defect_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("workerfail(output)")){ //작업자 파손(출고)
                    worst_item->setWorkerfail_output_count(count);
                    worst_item->setWorkerfail_output_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(probe)")){  //설비파손(프로브)
                    worst_item->setMachinefail_probe_count(count);
                    worst_item->setMachinefail_probe_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(eatching)")){  //설비파손(에칭)
                    worst_item->setMachinefail_eatching_rate(count);
                    worst_item->setMachinefail_eatching_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(light)")){  //설비파손(노광)
                    worst_item->setMachinefail_light_count(count);
                    worst_item->setMachinefail_light_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("machinefail(defect)")){  //설비파손(성막)
                    worst_item->setMachinefail_defect_count(count);
                    worst_item->setMachinefail_defect_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("padpaticle")){  //패트 이물
                    worst_item->setPadpaticle_count(count);
                    worst_item->setPadpaticle_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("etcpaticle")){  //이외 이물
                    worst_item->setEtcpaticle_count(count);
                    worst_item->setEtcpaticle_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("pattenpaticle")){  //패턴 이물
                    worst_item->setPattenpaticle_count(count);
                    worst_item->setPattenpaticle_rate(vaild_rate);
                }else if(DEFECT_ITEM_LIST.at(j)==tr("paticle")){  //이물
                    worst_item->setDefectpaticle_count(count);
                    worst_item->setDefectpaticle_rate(vaild_rate);
                }
            }
        }
    }

    double total_Workerfail_probe=0;
    double total_Workerfail_eatching=0;
    double total_Workerfail_light=0;
    double total_Workerfail_defect=0;
    double total_Workerfail_output=0;
    double total_Machinefail_probe=0;
    double total_Machinefail_eatching=0;
    double total_Machinefail_light=0;
    double total_Machinefail_defect=0;
    double total_Padpaticle=0;
    double total_Etcpaticle=0;
    double total_Pattenpaticle=0;
    double total_Defectpaticle=0;

    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        total_Workerfail_probe += worst_item->getWorkerfail_probe_rate();
        total_Workerfail_eatching += worst_item->getWorkerfail_eatching_rate();
        total_Workerfail_light += worst_item->getWorkerfail_light_rate();
        total_Workerfail_defect += worst_item->getWorkerfail_defect_rate();
        total_Workerfail_output += worst_item->getWorkerfail_output_rate();
        total_Machinefail_probe += worst_item->getMachinefail_probe_rate();
        total_Machinefail_eatching += worst_item->getMachinefail_eatching_rate();
        total_Machinefail_light += worst_item->getMachinefail_light_rate();
        total_Machinefail_defect += worst_item->getMachinefail_defect_rate();
        total_Padpaticle += worst_item->getPadpaticle_rate();
        total_Etcpaticle += worst_item->getEtcpaticle_rate();
        total_Pattenpaticle += worst_item->getPattenpaticle_rate();
        total_Defectpaticle += worst_item->getDefectpaticle_rate();
    }
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


    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        for(int j=0;j<REWORK_ITEM_LIST.count();j++){
            QString worstcount_query = QString("SELECT  SUM(REWORK_QTY)REWORK_SUM "
                                               "FROM [V_REWORK_LOTS] "
                                               "WHERE MOVEOUT_DTTM BETWEEN '%1' AND '%2' "
                                               "AND LOT_TYPE = 'A' "
                                               "AND MATERIAL_GROUP = 'CSP' "
                                               "AND MATERIAL_ID = '%3' "
                                               "AND REWORK_NAME = '%4' ")
                                                .arg(start_date_str).arg(end_date_str)
                                                .arg(item_vector.at(i)->getProduct_name()).arg(REWORK_ITEM_LIST.at(j));
            ms_query.exec(worstcount_query);

            if(ms_query.next()){
                int count = ms_query.value("REWORK_SUM").toInt();
                double vaild_rate;
                double temp1 = 100.0-daily_totalvild;
                double temp2 = worst_sum;
                vaild_rate = ((count/temp2)*temp1);
                if(REWORK_ITEM_LIST.at(j)==tr("paticle")){     //이물
                    worst_item->setRework_paticle_count(count);
                    worst_item->setRework_paticle_rate(vaild_rate);
                }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(defect)")){ //작업 미스(성막)
                    worst_item->setJobmiss_defect_count(count);
                    worst_item->setJobmiss_defect_rate(vaild_rate);
                }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(eatching)")){ //작업 미스(에칭)
                    worst_item->setJobmiss_eatching_count(count);
                    worst_item->setJobmiss_eatching_rate(vaild_rate);
                }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(light)")){ //작업 미스(성막)
                    worst_item->setJobmiss_light_count(count);
                    worst_item->setJobmiss_light_rate(vaild_rate);
                }else if(REWORK_ITEM_LIST.at(j)==tr("jobmiss(probe)")){ //작업미스(프로브)
                    worst_item->setJobmiss_probe_count(count);
                    worst_item->setJobmiss_probe_rate(vaild_rate);
                }
            }
        }
    }
    double total_Rework_paticle=0;
    double total_Jobmiss_defect=0;
    double total_Jobmiss_eatching=0;
    double total_Jobmiss_light=0;
    double total_Jobmiss_probe=0;

    for(int i=0;i<item_vector.count();i++){
        product_item *worst_item;
        worst_item = item_vector.at(i);
        total_Rework_paticle += worst_item->getRework_paticle_rate();
        total_Jobmiss_defect += worst_item->getJobmiss_defect_rate();
        total_Jobmiss_eatching += worst_item->getJobmiss_eatching_rate();
        total_Jobmiss_light += worst_item->getJobmiss_light_rate();
        total_Jobmiss_probe += worst_item->getJobmiss_probe_rate();
    }
    qDebug()<<"r total_Rework_paticle = "<<total_Rework_paticle;
    qDebug()<<"r total_Jobmiss_defect = "<<total_Jobmiss_defect;
    qDebug()<<"r total_Jobmiss_eatching = "<<total_Jobmiss_eatching;
    qDebug()<<"r total_Jobmiss_light = "<<total_Jobmiss_light;
    qDebug()<<"r total_Jobmiss_probe = "<<total_Jobmiss_probe;

}

void worst_search_excel_main::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
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
    workerfail_defect_count = 0;
    workerfail_defect_rate = 0;
    workerfail_output_count = 0;
    workerfail_output_rate = 0;

    machinefail_probe_count = 0;
    machinefail_probe_rate = 0;
    machinefail_eatching_count = 0;
    machinefail_eatching_rate = 0;
    machinefail_light_count = 0;
    machinefail_light_rate = 0;
    machinefail_defect_count = 0;
    machinefail_defect_rate = 0;

    defectpaticle_count=0;
    defectpaticle_rate=0;
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
    jobmiss_defect_count=0;
    jobmiss_defect_rate=0;
    rework_paticle_count=0;
    rework_paticle_rate =0;

}

double product_item::getJobmiss_defect_rate() const
{
    return jobmiss_defect_rate;
}

void product_item::setJobmiss_defect_rate(double value)
{
    jobmiss_defect_rate = value;
}

int product_item::getJobmiss_defect_count() const
{
    return jobmiss_defect_count;
}

void product_item::setJobmiss_defect_count(int value)
{
    jobmiss_defect_count = value;
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

double product_item::getDefectpaticle_rate() const
{
    return defectpaticle_rate;
}

void product_item::setDefectpaticle_rate(double value)
{
    defectpaticle_rate = value;
}

int product_item::getDefectpaticle_count() const
{
    return defectpaticle_count;
}

void product_item::setDefectpaticle_count(int value)
{
    defectpaticle_count = value;
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

double product_item::getMachinefail_defect_rate() const
{
    return machinefail_defect_rate;
}

void product_item::setMachinefail_defect_rate(double value)
{
    machinefail_defect_rate = value;
}

int product_item::getMachinefail_defect_count() const
{
    return machinefail_defect_count;
}

void product_item::setMachinefail_defect_count(int value)
{
    machinefail_defect_count = value;
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

double product_item::getWorkerfail_defect_rate() const
{
    return workerfail_defect_rate;
}

void product_item::setWorkerfail_defect_rate(double value)
{
    workerfail_defect_rate = value;
}

int product_item::getWorkerfail_defect_count() const
{
    return workerfail_defect_count;
}

void product_item::setWorkerfail_defect_count(int value)
{
    workerfail_defect_count = value;
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
