#include "worst_search_excel_main.h"
#include "ui_worst_search_excel_main.h"

worst_search_excel_main::worst_search_excel_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::worst_search_excel_main)
{
    ui->setupUi(this);
    QString configini_str = qApp->applicationDirPath()+"/worst_excel_setting.ini";
    QString filepathCSP;
    QString filepathWLP;
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setting");
    filepathCSP = settings.value("worst_excel_file").toString();
    filepathWLP = settings.value("worst_WLP_excel_file").toString();
    settings.endGroup();
    ui->LE_filepath->setText(filepathCSP);
    ui->LE_WLP_filepath->setText(filepathWLP);
    ui->select_data->setDate(QDate::currentDate().addDays(-1));

    QString configini_str_server = qApp->applicationDirPath()+"/server.ini";
    QSettings settings_server(configini_str_server,QSettings::IniFormat);
    settings_server.beginGroup("setverset");
    QString server_ip = settings_server.value("serverip").toString();
    QString db_port = settings_server.value("dbport").toString();
    QString ftp_port = settings_server.value("ftpport").toString();
    settings_server.endGroup();
    QString mydb_name = QString("CSP_MY_MESDB_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

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
    CSP_model = new QSqlTableModel(this,my_mesdb);
    CSP_model->setTable("MES_Worst_CSP_Process");
    CSP_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    CSP_model->select();

    ui->CSP_Process->setModel(CSP_model);

    WLP_model = new QSqlTableModel(this,my_mesdb);
    WLP_model->setTable("MES_Worst_WLP_Process");
    WLP_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    WLP_model->select();

    ui->WLP_Process->setModel(WLP_model);

    litedb = QSqlDatabase::addDatabase("QSQLITE","CSP_WSP_workplan");
    QString litedbname = qApp->applicationDirPath()+"/CSP_WSP_workplan.db";
    litedb.setDatabaseName(litedbname);
    if(!litedb.open()){
        qDebug()<<"litedb open false";
    }
    QSqlQuery litequery(litedb);
    litequery.exec("CREATE TABLE IF NOT EXISTS [work_plan]( "
                   "[plan_time] TIME); ");

    work_plan_model = new QSqlTableModel(this,litedb);
    work_plan_model->setTable("work_plan");
    work_plan_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    work_plan_model->select();

    ui->work_plan_timeedit->setTime(QTime::currentTime());
    ui->work_plan_table->setModel(work_plan_model);

    connect(&work_plan_timer,SIGNAL(timeout()),this,SLOT(work_plan_timer_timeout()));
    work_plan_timer.setInterval(300);
    work_plan_timer.start();

    type_price_model = new QSqlTableModel(this,my_mesdb);
    type_price_model->setTable("MATERIAL_GROUP1");
    type_price_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    type_price_model->select();

    ui->type_price_table->setModel(type_price_model);


}

worst_search_excel_main::~worst_search_excel_main()
{
    delete ui;
}

void worst_search_excel_main::on_start_btn_clicked()
{
    if(ui->select_data->date().day()== 1){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "from",
                                                                     tr("1day continue because remove ago monuth data marge and remove?\n"),
                                                                     QMessageBox::Cancel| QMessageBox::Yes);
        if(resBtn != QMessageBox::Yes){
            return;
        }
    }
    if(ui->LE_filepath->text() != ""){
    worst_search_th *th = new worst_search_th();
    th->setSelect_date(ui->select_data->date());
    th->start();
    connect(th,SIGNAL(sig_excel_work()),this,SLOT(slot_excel_work()));
    connect(th,SIGNAL(sig_debug_output(QString)),this,SLOT(slot_debug_output(QString)));
    }
    if(ui->LE_WLP_filepath->text() != ""){
    worst_search_WLP_th *th_WLP = new worst_search_WLP_th();
    th_WLP->setSelect_date(ui->select_data->date());
    th_WLP->start();
    connect(th_WLP,SIGNAL(sig_debug_output(QString)),this,SLOT(slot_debug_output_WLP(QString)));
    connect(th_WLP,SIGNAL(sig_excel_work()),this,SLOT(slot_excel_WLP_work()));
    }

}

void worst_search_excel_main::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
}

void worst_search_excel_main::slot_excel_work()
{

    worst_search_th *th = (worst_search_th *)sender();
    QAxObject* excel = new QAxObject( "Excel.Application",0);
    excel->dynamicCall("SetVisible(bool)",true);
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)",ui->LE_filepath->text());
    QAxObject *sheet_fab10 = workbook->querySubObject( "Worksheets(QString)",tr("FAB10worstrate"));
    QAxObject *sheet_group = workbook->querySubObject( "Worksheets(QString)",tr("groupvaild"));
    QAxObject *sheet_rework = workbook->querySubObject( "Worksheets(QString)",tr("reworkhistory"));
    QAxObject *sheet_destoryhistory = workbook->querySubObject( "Worksheets(QString)",tr("distoryhistory"));
    QAxObject *sheet_exteriorcheck = workbook->querySubObject( "Worksheets(QString)",tr("exteriorcheck"));
    QAxObject *sheet_destory = workbook->querySubObject( "Worksheets(QString)",tr("distory"));
    QAxObject *sheet_limitproductvaild = workbook->querySubObject( "Worksheets(QString)",tr("limitproductvaild"));


    if(ui->select_data->date().day()== 1){  //매월 1일 초기화 작업

        QAxObject *insert_cell;
        QAxObject *select_cell= sheet_fab10->querySubObject("Cells(int,int)",27,16000);
        QAxObject *select_cell1;
        QAxObject *select_cell2;
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("offset(int,int)",0,1);
        insert_cell = select_cell->querySubObject("EntireColumn()");
        insert_cell->dynamicCall("Insert(int,int)",-4161,0);
        select_cell = select_cell->querySubObject("offset(int,int)",0,-1);
        QString temp_month = QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month"));
        select_cell->setProperty("Value",temp_month);
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,-1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",1,2);
        select_cell1->dynamicCall("Merge(bool)",true);
        select_cell1 = select_cell->querySubObject("offset(int,int)",1,-1);
        select_cell1 = select_cell1->querySubObject("Columns()");
        select_cell1->setProperty("Hidden",true);
        select_cell1 = select_cell->querySubObject("Offset(int,int)",1,0);
        select_cell1 = select_cell1->querySubObject("resize(int,int)",14,1);
        select_cell2 = select_cell->querySubObject("offset(int,int)",0,32)->querySubObject("Offset(int,int)",1,0)->querySubObject("resize(int,int)",14,1);
        select_cell2->dynamicCall("Copy");
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell2 = select_cell->querySubObject("offset(int,int)",16,0)->querySubObject("end(int)",-4161)->querySubObject("end(int)",-4161);
        select_cell2->dynamicCall("Copy");
        select_cell1 = select_cell->querySubObject("offset(int,int)",16,0);
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);



        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        int temp_column1 = select_cell1->dynamicCall("Column").toInt();
        int temp_row1 = select_cell1->dynamicCall("Row").toInt();
        select_cell1 = select_cell->querySubObject("end(int)",-4161);
        int temp_column2 = select_cell1->dynamicCall("Column").toInt();
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("end(int)",-4121);
        int temp_row2 = select_cell1->dynamicCall("Row").toInt();
        int column_size = temp_column2-temp_column1+1;
        int row_size = temp_row2-temp_row1+1;
//        qDebug()<<"row_size = "<<row_size<<"column_size = "<<column_size;
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",row_size,31);
        select_cell1->dynamicCall("ClearContents()");
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,1);
        select_cell1->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);


        int cal_day = ui->select_data->date().addMonths(1).daysTo(ui->select_data->date());
        cal_day = cal_day * -1;
        for(int i=1;i<=cal_day;i++){
            select_cell1 = select_cell->querySubObject("offset(int,int)",0,i);
            select_cell1->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,32);
        select_cell1->setProperty("Value",tr("accmulate"));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,31);
        QAxObject *chart_object = sheet_fab10->querySubObject("ChartObjects(int)",1);
        chart_object = chart_object->querySubObject("Chart");
        QAxObject *SeriesCollection = chart_object->querySubObject("SeriesCollection(int)",1);
        QString chart_excel_start_address = select_cell->dynamicCall("Address").toString();
        QString chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        int chart_last_column = select_cell1->dynamicCall("Columns").toInt();
        SeriesCollection->setProperty("XValues",QString("=%1!%2:%3").arg(tr("FAB10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));
        int column_excel = select_cell->dynamicCall("Column").toInt();
        select_cell1 = sheet_fab10->querySubObject("Cells(int,int)",42,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("daily"));
        SeriesCollection->setProperty("Values",QString("=%1!%2:%3").arg(tr("FAB10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        select_cell1 = sheet_fab10->querySubObject("Cells(int,int)",43,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("accmulate"));
        SeriesCollection->setProperty("Values",QString("=%1!%2:%3").arg(tr("FAB10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        QAxObject *select_cell3 = sheet_group->querySubObject("Cells(int,int)",25,2);
        QAxObject *select_cell4;
        select_cell3 = select_cell3->querySubObject("Resize(int,int)",4,31);
        select_cell3->dynamicCall("ClearContents");
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",24,2);
        select_cell3->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",25,1);
        for(int i=1;i<=cal_day;i++){
            select_cell4 = select_cell3->querySubObject("offset(int,int)",0,i);
            select_cell4->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }

        select_cell3 = sheet_group->querySubObject("Cells(int,int)",31,2);
        select_cell3 = select_cell3->querySubObject("Resize(int,int)",4,31);
        select_cell3->dynamicCall("ClearContents");
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",30,2);
        select_cell3->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",31,1);
        for(int i=1;i<=cal_day;i++){
            select_cell4 = select_cell3->querySubObject("offset(int,int)",0,i);
            select_cell4->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }


        select_cell3 = sheet_group->querySubObject("Cells(int,int)",37,2);
        select_cell3 = select_cell3->querySubObject("Resize(int,int)",4,31);
        select_cell3->dynamicCall("ClearContents");
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",36,2);
        select_cell3->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell3 = sheet_group->querySubObject("Cells(int,int)",37,1);
        for(int i=1;i<=cal_day;i++){
            select_cell4 = select_cell3->querySubObject("offset(int,int)",0,i);
            select_cell4->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }

        QAxObject *select_cell5 = sheet_rework->querySubObject("Cells(int,int)",1,1);
        select_cell5->dynamicCall("AutoFilter(int)",1);

        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5 =select_cell5->querySubObject("end(int)",-4121);
        int temp_rowcount1 = select_cell5->dynamicCall("Row").toInt();
        int temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5 = select_cell5->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell5->dynamicCall("ClearContents");
        select_cell5->dynamicCall("ClearFormats");
        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("Entirecolumn()");
        select_cell5->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));
        QAxObject *select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",1,1);
        select_cell6->dynamicCall("AutoFilter(int)",1);
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",2,1);
        select_cell6 =select_cell6->querySubObject("end(int)",-4121);
        temp_rowcount1 = select_cell6->dynamicCall("Row").toInt();
        temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",2,1);
        select_cell6 = select_cell6->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell6->dynamicCall("ClearContents");
        select_cell6->dynamicCall("ClearFormats");
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("Entirecolumn()");
        select_cell6->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));

        QAxObject *select_cell7 = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,2);
        QAxObject *select_cell8;
        QAxObject *select_cell9;
        QAxObject *select_cell10;
        int i = 1;
        int temp_data_column_count = 0;
        while(true){
            select_cell8 =select_cell7->querySubObject("offset(int,int)",0,i);
            QString value = select_cell8->dynamicCall("Value").toString();
            if(value.indexOf(tr("month"))<0){
                temp_data_column_count = i;
                break;
            }
            i++;
        }

        select_cell9 = select_cell8->querySubObject("EntireColumn");
        select_cell9->dynamicCall("Insert(int,int)",-4161,0);
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-1);
        select_cell9->setProperty("Value",QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month")));
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-2)->querySubObject("EntireColumn");
        select_cell9->setProperty("Hidden",true);

        select_cell9  = select_cell8->querySubObject("offset(int,int)",1,31);
        select_cell9 = select_cell9->querySubObject("resize(int,int)",6,1);
        select_cell9->dynamicCall("copy");

        select_cell10 = select_cell8->querySubObject("offset(int,int)",1,-1);
        select_cell10->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell9 = select_cell8->querySubObject("resize(int,int)",6,31);
        select_cell9->dynamicCall("ClearContents");



        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            select_cell9->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i).toString("yyyy-MM-dd")));
        }

        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            int today =  ui->select_data->date().day();
            int search_day = select_cell9->dynamicCall("Value").toDate().day();
            if(search_day<=10){
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",false);
            }else {
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",true);
            }
        }

        QAxObject *select_cell11 = sheet_destory->querySubObject("Cells(int,int)",26,2);
        QAxObject *select_cell12;
        QAxObject *select_cell13;
        i = 1;
        while(true){
            select_cell12 =select_cell11->querySubObject("offset(int,int)",0,i);
            QString value = select_cell12->dynamicCall("Value").toString();
            if(value.indexOf(tr("month"))<0){
                temp_data_column_count = i;
                break;
            }
            i++;
        }


        select_cell13 = select_cell12->querySubObject("EntireColumn");
        select_cell13->dynamicCall("Insert(int,int)",-4161,0);
        select_cell13 = select_cell12->querySubObject("offset(int,int)",0,-1);
        select_cell13->setProperty("Value",QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month")));
        select_cell13 = select_cell12->querySubObject("offset(int,int)",0,-2)->querySubObject("EntireColumn");
        select_cell13->setProperty("Hidden",true);

        select_cell13  = select_cell12->querySubObject("offset(int,int)",0,31)->querySubObject("offset(int,int)",1,0);
        select_cell13 = select_cell13->querySubObject("resize(int,int)",6,1);
        select_cell13->dynamicCall("copy");

        select_cell13 = select_cell12->querySubObject("offset(int,int)",1,-1);
        select_cell13->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell13  = select_cell12->querySubObject("offset(int,int)",0,31)->querySubObject("offset(int,int)",8,0);
        select_cell13 = select_cell13->querySubObject("resize(int,int)",6,1);
        select_cell13->dynamicCall("copy");

        select_cell13 = select_cell12->querySubObject("offset(int,int)",8,-1);
        select_cell13->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell13 = select_cell12->querySubObject("resize(int,int)",5,31);
        select_cell13->dynamicCall("ClearContents");

        for(int i=1;i<=cal_day;i++){
            select_cell13 = select_cell12->querySubObject("offset(int,int)",0,i-1);
            select_cell13->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i).toString("yyyy-MM-dd")));
        }
        int select_cell12_column = select_cell12->dynamicCall("Column").toInt();
        select_cell12 = sheet_destory->querySubObject("Cells(int,int)",33,select_cell12_column);
        select_cell13 = select_cell12->querySubObject("offset(int,int)",0,-1);
        select_cell13->setProperty("Value",QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month")));
        select_cell13 = select_cell12->querySubObject("resize(int,int)",5,31);
        select_cell13->dynamicCall("ClearContents");
        for(int i=1;i<=cal_day;i++){
            select_cell13 = select_cell12->querySubObject("offset(int,int)",0,i-1);
            select_cell13->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i).toString("yyyy-MM-dd")));
        }
        QAxObject *select_cell14 = sheet_limitproductvaild->querySubObject("Cells(int,int)",4,2);
        QAxObject *select_cell15;
        QAxObject *select_cell16;
        i=1;
        int select_cell15_base_column = 0;
        while(true){
            select_cell15 = select_cell14->querySubObject("offset(int,int)",0,i);
            if(select_cell15->dynamicCall("Value").toDate().day() == 1){
                select_cell15_base_column = i;
                break;
            }
            i++;
        }
        select_cell16 = select_cell15->querySubObject("EntireColumn()");
        select_cell16->dynamicCall("Insert(int,int)",-4161,0);
        select_cell16 = select_cell15->querySubObject("offset(int,int)",0,-1);
        select_cell16->setProperty("Value",temp_month);
        select_cell16 = select_cell16->querySubObject("offset(int,int)",0,-1)->querySubObject("EntireColumn()");
        select_cell16->setProperty("Hidden",true);

        select_cell16  = select_cell15->querySubObject("offset(int,int)",0,31)->querySubObject("offset(int,int)",1,0);
        select_cell16 = select_cell16->querySubObject("resize(int,int)",3,1);
        select_cell16->dynamicCall("copy");

        select_cell16 = select_cell15->querySubObject("offset(int,int)",1,-1);
        select_cell16->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell16 = select_cell15->querySubObject("resize(int,int)",4,31);
        select_cell16->dynamicCall("ClearContents");

        for(int i=0;i<cal_day;i++){
            select_cell16 = select_cell15->querySubObject("offset(int,int)",0,i);
            select_cell16->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i+1).toString("yyyy-MM-dd")));
            select_cell16->querySubObject("EntireColumn()")->setProperty("Hidden",true);
        }


    }


    QAxObject *modify_cell = sheet_fab10->querySubObject("Cells(int,int)",26,2);

    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    QString month_str = modify_cell->dynamicCall("Value()").toString().split(tr("month")).at(0);
    int write_month = month_str.toInt();
    if(write_month = ui->select_data->date().month()){
        qDebug()<<"match month";
        qDebug()<<write_month;
    }

    int i=2;
    int cellcolumn = 0;
    while(true){
        modify_cell = sheet_fab10->querySubObject("Cells(int,int)",27,i);
        QString excel_day = modify_cell->dynamicCall("Value()").toString();

        QString user_day = QString("%1%2").arg(ui->select_data->date().day()).arg(tr("day"));
        if(excel_day == user_day){
            cellcolumn = modify_cell->dynamicCall("Column()").toInt();
            qDebug()<<"select day cell cellcolumn = "<<cellcolumn;
            break;
        }
        i++;
    }
    double daily_total_sum = 0.0;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP003_worst());
    daily_total_sum += th->getTotal_DP003_worst();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP004_worst());
    daily_total_sum += th->getTotal_DP004_worst();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP006_worst());
    daily_total_sum += th->getTotal_DP006_worst();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP001_worst());
    daily_total_sum += th->getTotal_DP001_worst();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP005_worst());
    daily_total_sum += th->getTotal_DP005_worst();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP008_worst());
    daily_total_sum += th->getTotal_DP008_worst();
    double totalmachine_fail = th->getTotal_Machinefail_defect()+th->getTotal_Machinefail_eatching()
                                +th->getTotal_Machinefail_light()+th->getTotal_Machinefail_probe();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",totalmachine_fail);
    daily_total_sum += totalmachine_fail;
    double totalworker_fail = th->getTotal_Workerfail_defect()+th->getTotal_Workerfail_light()+th->getTotal_Workerfail_eatching()
                                +th->getTotal_Workerfail_output()+th->getTotal_Workerfail_probe();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",totalworker_fail);
    daily_total_sum += totalworker_fail;
    double total_prcdspecout = th->getTotal_prcdlow()+th->getTotal_prcdhigh();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",total_prcdspecout);
    daily_total_sum += total_prcdspecout;
    double total_metalspecout = th->getTotal_metallow()+th->getTotal_metalhigh();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",total_metalspecout);
    daily_total_sum += total_metalspecout;
    double totalpaticle= th->getTotal_Defectpaticle()+th->getTotal_Etcpaticle()
                            +th->getTotal_Padpaticle()+th->getTotal_Pattenpaticle()+th->getTotal_Rework_paticle();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",totalpaticle);
    daily_total_sum += totalpaticle;
    double totaljobmiss = th->getTotal_Jobmiss_defect()+th->getTotal_Jobmiss_eatching()
                            +th->getTotal_Jobmiss_light()+th->getTotal_Jobmiss_probe();
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",totaljobmiss);
    daily_total_sum += totaljobmiss;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",40,cellcolumn);
    modify_cell->setProperty("Value",daily_total_sum);
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getLast_probe_vild_daily());
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",42,cellcolumn);
    modify_cell->setProperty("Value",th->getDaily_totalvild());
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",43,cellcolumn);
    modify_cell->setProperty("Value",th->getAccumulate_totalvild());


    modify_cell = modify_cell->querySubObject("EntireColumn");
    modify_cell->setProperty("Hidden",false);

    //누적
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    cellcolumn = modify_cell->dynamicCall("Column()").toInt();
    double accumulate_daily_total_sum = 0.0;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP003_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP003_worst;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP004_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP004_worst;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP006_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP006_worst;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP001_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP001_worst;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP005_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP005_worst;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP008_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP008_worst;
    double accumulate_totalmachine_fail = th->accumulate_total_Machinefail_defect+th->accumulate_total_Machinefail_eatching
                                +th->accumulate_total_Machinefail_light+th->accumulate_total_Machinefail_probe;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalmachine_fail);
    accumulate_daily_total_sum += accumulate_totalmachine_fail;
    double accumulate_totalworker_fail = th->accumulate_total_Workerfail_defect+th->accumulate_total_Workerfail_light+th->accumulate_total_Workerfail_eatching
                                +th->accumulate_total_Workerfail_output+th->accumulate_total_Workerfail_probe;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalworker_fail);
    accumulate_daily_total_sum += accumulate_totalworker_fail;
    double accumulate_total_prcdspecout = th->accumulate_total_prcdlow+th->accumulate_total_prcdhigh;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",accumulate_total_prcdspecout);
    accumulate_daily_total_sum += accumulate_total_prcdspecout;
    double accumulate_total_metalspecout = th->accumulate_total_metallow+th->accumulate_total_metalhigh;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",accumulate_total_metalspecout);
    accumulate_daily_total_sum += accumulate_total_metalspecout;
    double accumulate_totalpaticle= th->accumulate_total_Defectpaticle+th->accumulate_total_Etcpaticle
                            +th->accumulate_total_Padpaticle+th->accumulate_total_Pattenpaticle+th->accumulate_total_Rework_paticle;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalpaticle);
    accumulate_daily_total_sum += accumulate_totalpaticle;
    double accumulate_totaljobmiss = th->accumulate_total_Jobmiss_defect+th->accumulate_total_Jobmiss_eatching
                            +th->accumulate_total_Jobmiss_light+th->accumulate_total_Jobmiss_probe;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totaljobmiss);
    accumulate_daily_total_sum += accumulate_totaljobmiss;
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",40,cellcolumn);
    modify_cell->setProperty("Value",accumulate_daily_total_sum);
    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_last_probe_vild_daily);

    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",43,cellcolumn);
    modify_cell->setProperty("Value",th->getAccumulate_totalvild());



    i=2;
    cellcolumn = 0;
    while(true){
        modify_cell = sheet_group->querySubObject("Cells(int,int)",25,i);
        QString excel_day = modify_cell->dynamicCall("Value()").toString();

        QString user_day = QString("%1%2").arg(ui->select_data->date().day()).arg(tr("day"));
        if(excel_day == user_day){
            cellcolumn = modify_cell->dynamicCall("Column()").toInt();
            qDebug()<<"select day cell cellcolumn = "<<cellcolumn;
            break;
        }
        i++;
    }
    modify_cell = sheet_group->querySubObject("Cells(int,int)",26,cellcolumn);
    modify_cell->setProperty("Value",th->getSingle_last_probevild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",27,cellcolumn);
    modify_cell->setProperty("Value",th->getDPX_last_probevild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->getRSM_last_probevild());

    modify_cell = sheet_group->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->getSingle_daily_totalvild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->getDPX_daily_totalvild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",th->getRSM_daily_totalvild());

    modify_cell = sheet_group->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",th->getSingle_accumulate_totalvild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",th->getDPX_accumulate_totalvild());
    modify_cell = sheet_group->querySubObject("Cells(int,int)",40,cellcolumn);
    modify_cell->setProperty("Value",th->getRSM_accumulate_totalvild());

    modify_cell =  sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
    modify_cell->dynamicCall("AutoFilter(int)",1);

    modify_cell = sheet_rework->querySubObject("Cells(int,int)",3,1);
    modify_cell = modify_cell->querySubObject("end(int)",-4121);
    int rework_write_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
    if(ui->select_data->date().day()==1){
        rework_write_point_row=2;
    }
    if(rework_write_point_row > 10000){
        rework_write_point_row = 2;
    }
    QString temp_process_type = "";
    int marge_count = 0;
    for(int j=0;j<th->getRework_worstcpsprocesslist().count();j++){
        rework_text_type temp_type = th->getRework_worstcpsprocesslist().at(j);
        QAxObject *rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,1);
        rework_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));
        double temp_data1 = 100.0-(double)th->getDaily_totalvild();
        double temp_data2 = (double)temp_type.getChip_count()/(double)th->getWorst_sum();
        double worst_rate = temp_data2 *temp_data1;

        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,2);
        rework_write_cell->setProperty("Value",RoundOff(worst_rate,2));
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,3);
        rework_write_cell->setProperty("Value",temp_type.getProduct_name());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,4);
        rework_write_cell->setProperty("Value",temp_type.getLOT());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,5);
        rework_write_cell->setProperty("Value",temp_type.getProcess());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,6);
        rework_write_cell->setProperty("Value",temp_type.getLot_count());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,7);
        rework_write_cell->setProperty("Value",temp_type.getChip_count());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,8);
        rework_write_cell->setProperty("Value",temp_type.getContent());
    }

     modify_cell =  sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     QString selection_date = QString("%1%2 %3%4").arg(ui->select_data->date().month()).arg(tr("month"))
                                                  .arg(ui->select_data->date().day()).arg(tr("day"));

     modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);

     modify_cell =  sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     modify_cell->dynamicCall("AutoFilter(int)",1);

     modify_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",1,1);
     modify_cell = modify_cell->querySubObject("end(int)",-4121);
     int distoryhistory_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
     if(ui->select_data->date().day()==1){
         distoryhistory_point_row=2;
     }
     if(distoryhistory_point_row>10000){
        distoryhistory_point_row=2;
     }
     for(int j=0;j<th->getDefect_worstcpsprocesslist().count();j++){
         defect_worst_type defect_item;
         defect_item = th->defect_worstcpsprocesslist.at(j);
         QAxObject *distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,1);
         distoryhistory_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));

         double temp_data1 = 100.0-(double)th->getDaily_totalvild();
         double temp_data2 = (double)defect_item.getChip_count()/(double)th->getWorst_sum();

         double worst_rate =  temp_data2 *temp_data1;
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,2);
         distoryhistory_write_cell->setProperty("Value",RoundOff(worst_rate,2));
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,3);
         distoryhistory_write_cell->setProperty("Value",defect_item.getProduct_name());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,4);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLOT());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,5);
         distoryhistory_write_cell->setProperty("Value",defect_item.getDefect_name());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,6);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLot_count());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,7);
         distoryhistory_write_cell->setProperty("Value",defect_item.getChip_count());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,8);
         QSqlQuery my_query1(my_mesdb);
         my_query1.exec(QString("select * from MATERIAL_GROUP1 where type = '%1'").arg(defect_item.getMetarial_grouptype()));
         if(my_query1.next()){
             double temp_result = my_query1.value("price").toDouble() * (double)defect_item.getLot_count();
             distoryhistory_write_cell->setProperty("Value",temp_result);
         }else {
             distoryhistory_write_cell->setProperty("Value",defect_item.getMetarial_grouptype());
         }

         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,9);
         distoryhistory_write_cell->setProperty("Value",defect_item.getContent());
     }
     modify_cell =  sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",215,10);

     modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);


     modify_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,2);
     i = 2;
     while(true){
         modify_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,i);
         QString exceldate = modify_cell->dynamicCall("Value()").toDate().toString("yyyy-MM-dd");
         if(exceldate == ui->select_data->date().toString("yyyy-MM-dd")){
             break;
         }
         i++;
     }
     int exteriorcheck_colum = i;

     QAxObject *exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",18,exteriorcheck_colum);
     exteriorcheck_cell->setProperty("Value",th->getExterior_pattenpaticle_vaild());
     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",19,exteriorcheck_colum);
     exteriorcheck_cell->setProperty("Value",th->getExterior_padpaticle_vaild());
     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",20,exteriorcheck_colum);
     exteriorcheck_cell->setProperty("Value",th->getExterior_sindiseatching_vaild());
     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",21,exteriorcheck_colum);
     exteriorcheck_cell->setProperty("Value",th->getExterior_etcpaticle_vaild());
     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",22,exteriorcheck_colum);
     exteriorcheck_cell->setProperty("Value",th->getExterior_brigitpadworst_vaild());

     exteriorcheck_cell = exteriorcheck_cell->querySubObject("EntireColumn");
     exteriorcheck_cell->setProperty("Hidden",false);
//    장은지씨 물어 보고 작업
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",18,exteriorcheck_colum)->querySubObject("end(int)",-4161);

//     exteriorcheck_cell = exteriorcheck_cell->querySubObject("offset(int,int)",0,1);
//     exteriorcheck_colum = exteriorcheck_cell->dynamicCall("Column").toInt();
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",18,exteriorcheck_colum);
//     exteriorcheck_cell->setProperty("Value",th->accumulate_exterior_pattenpaticle_vaild);
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",19,exteriorcheck_colum);
//     exteriorcheck_cell->setProperty("Value",th->accumulate_exterior_padpaticle_vaild);
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",20,exteriorcheck_colum);
//     exteriorcheck_cell->setProperty("Value",th->accumulate_exterior_sindiseatching_vaild);
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",21,exteriorcheck_colum);
//     exteriorcheck_cell->setProperty("Value",th->accumulate_exterior_etcpaticle_vaild);
//     exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",22,exteriorcheck_colum);
//     exteriorcheck_cell->setProperty("Value",th->accumulate_exterior_brigitpadworst_vaild);



     QAxObject *destory_cell = sheet_destory->querySubObject("Cells(int,int)",26,2);
     i=2;
     while(true){
          destory_cell = sheet_destory->querySubObject("Cells(int,int)",26,i);
          QString exceldate = destory_cell->dynamicCall("Value()").toDate().toString("yyyy-MM-dd");
          if(exceldate == ui->select_data->date().toString("yyyy-MM-dd")){
              break;
          }
          i++;
     }
     int destory_colum = i;
     int machine_fail_depostion_count = 0;
     int machine_fail_light_count = 0;
     int machine_fail_eathing_count = 0;
     int machine_fail_probe_count = 0;
     int worker_fail_depostion_count = 0;
     int worker_fail_light_count = 0;
     int worker_fail_eathing_count = 0;
     int worker_fail_probe_count = 0;
     int worker_fail_output_count = 0;
     for(int j=0;j<th->getDefect_worstcpsprocesslist().count();j++){
        defect_worst_type defect_item = th->getDefect_worstcpsprocesslist().at(j);
        if(defect_item.getDefect_name() == tr("machinefail(probe)")){
            machine_fail_probe_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("machinefail(eatching)")){
            machine_fail_eathing_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("machinefail(light)")){
            machine_fail_light_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("machinefail(deposition)")){
            machine_fail_depostion_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("workerfail(probe)")){
            worker_fail_probe_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("workerfail(eatching)")){
            worker_fail_eathing_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("workerfail(light)")){
            worker_fail_light_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("workerfail(deposition)")){
            worker_fail_depostion_count += defect_item.getLot_count();
        }else if(defect_item.getDefect_name() == tr("workerfail(output)")){
            worker_fail_output_count += defect_item.getLot_count();
        }
     }
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",27,destory_colum);
     destory_cell->setProperty("Value",machine_fail_depostion_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",28,destory_colum);
     destory_cell->setProperty("Value",machine_fail_light_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",29,destory_colum);
     destory_cell->setProperty("Value",machine_fail_eathing_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",30,destory_colum);
     destory_cell->setProperty("Value",machine_fail_probe_count);

     destory_cell = sheet_destory->querySubObject("Cells(int,int)",34,destory_colum);
     destory_cell->setProperty("Value",worker_fail_depostion_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",35,destory_colum);
     destory_cell->setProperty("Value",worker_fail_light_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",36,destory_colum);
     destory_cell->setProperty("Value",worker_fail_eathing_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",37,destory_colum);
     destory_cell->setProperty("Value",worker_fail_probe_count);
     destory_cell = sheet_destory->querySubObject("Cells(int,int)",38,destory_colum);
     destory_cell->setProperty("Value",worker_fail_output_count);

     i = 2;
     int csp_wirte_column =0;
     while(true){
         modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",4,i);
         QString exceldate = modify_cell->dynamicCall("Value()").toDate().toString("yyyy-MM-dd");
         if(exceldate == ui->select_data->date().toString("yyyy-MM-dd")){
            csp_wirte_column = i;
            modify_cell->querySubObject("EntireColumn")->setProperty("Hidden",false);
             break;
         }
         i++;
     }

     for(int i=0;i<th->getCSP_Limit_product_item_list().count();i++){
        csp_limit_product_type item;
        item = th->getCSP_Limit_product_item_list().at(i);
        if(item.getProduct_name()=="X897WYF-TXBCG"){
            modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",5,csp_wirte_column);
            modify_cell->setProperty("Value",item.getVaild());
        }else if(item.getProduct_name()=="X897WYF-TXBDG"){
            modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",6,csp_wirte_column);
            modify_cell->setProperty("Value",item.getVaild());
        }else if(item.getProduct_name()=="X897BYF-TXBCG"){
            modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",7,csp_wirte_column);
            modify_cell->setProperty("Value",item.getVaild());
        }
     }

     workbook->dynamicCall("Save");
     excel->dynamicCall("Quit()");
}

void worst_search_excel_main::slot_excel_WLP_work()
{
    worst_search_WLP_th *th = (worst_search_WLP_th *)sender();
    QAxObject* excel = new QAxObject( "Excel.Application",0);
    excel->dynamicCall("SetVisible(bool)",true);
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)",ui->LE_WLP_filepath->text());
    QAxObject *sheet_wlp10 = workbook->querySubObject( "Worksheets(QString)",tr("WLP10worstrate"));
    QAxObject *sheet_wlp10_limit = workbook->querySubObject( "Worksheets(QString)",tr("WLP10worstrate_limit"));
    QAxObject *sheet_rework = workbook->querySubObject( "Worksheets(QString)",tr("a_reworkhistory"));
    QAxObject *sheet_b_rework = workbook->querySubObject( "Worksheets(QString)",tr("b_reworkhistory"));
    QAxObject *sheet_destoryhistory = workbook->querySubObject( "Worksheets(QString)",tr("a_distoryhistory"));
    QAxObject *sheet_b_destoryhistory = workbook->querySubObject( "Worksheets(QString)",tr("b_distoryhistory"));
    QAxObject *sheet_exteriorcheck = workbook->querySubObject( "Worksheets(QString)",tr("WLP_exteriorcheck"));
    QAxObject *sheet_limitproductvaild = workbook->querySubObject( "Worksheets(QString)",tr("exterior_limitproductvaild"));


    if(ui->select_data->date().day()== 1){

        QAxObject *insert_cell;
        QAxObject *select_cell= sheet_wlp10->querySubObject("Cells(int,int)",27,16000);
        QAxObject *select_cell1;
        QAxObject *select_cell2;
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("offset(int,int)",0,1);




        insert_cell = select_cell->querySubObject("EntireColumn()");
        insert_cell->dynamicCall("Insert(int,int)",-4161,0);



        select_cell = select_cell->querySubObject("offset(int,int)",0,-1);
        QString temp_month = QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month"));
        select_cell->setProperty("Value",temp_month);
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,-1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",1,2);
        select_cell1->dynamicCall("Merge(bool)",true);
        select_cell1 = select_cell->querySubObject("offset(int,int)",1,-1);
        select_cell1 = select_cell1->querySubObject("Columns()");
        select_cell1->setProperty("Hidden",true);

        select_cell1 = select_cell->querySubObject("Offset(int,int)",1,0);
        select_cell1 = select_cell1->querySubObject("resize(int,int)",14,1);
        select_cell2 = select_cell->querySubObject("offset(int,int)",0,32)->querySubObject("Offset(int,int)",1,0)->querySubObject("resize(int,int)",14,1);
        select_cell2->dynamicCall("Copy");
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell2 = select_cell->querySubObject("offset(int,int)",16,0)->querySubObject("end(int)",-4161)->querySubObject("end(int)",-4161);
        select_cell2->dynamicCall("Copy");
        select_cell1 = select_cell->querySubObject("offset(int,int)",16,0);
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        int temp_column1 = select_cell1->dynamicCall("Column").toInt();
        int temp_row1 = select_cell1->dynamicCall("Row").toInt();
        select_cell1 = select_cell->querySubObject("end(int)",-4161);
        int temp_column2 = select_cell1->dynamicCall("Column").toInt();
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("end(int)",-4121);
        int temp_row2 = select_cell1->dynamicCall("Row").toInt();
        int column_size = temp_column2-temp_column1+1;
        int row_size = temp_row2-temp_row1+1;
//        qDebug()<<"row_size = "<<row_size<<"column_size = "<<column_size;
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",15,31);
        select_cell1->dynamicCall("ClearContents()");
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,1);
        select_cell1->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);

        int cal_day = ui->select_data->date().addMonths(1).daysTo(ui->select_data->date());
        cal_day = cal_day * -1;
        for(int i=1;i<=cal_day;i++){
            select_cell1 = select_cell->querySubObject("offset(int,int)",0,i);
            select_cell1->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,32);
        select_cell1->setProperty("Value",tr("accmulate"));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,31);
        QAxObject *chart_object = sheet_wlp10->querySubObject("ChartObjects(int)",1);
        chart_object = chart_object->querySubObject("Chart");
        QAxObject *SeriesCollection = chart_object->querySubObject("SeriesCollection(int)",1);
        QString chart_excel_start_address = select_cell->dynamicCall("Address").toString();
        QString chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        int chart_last_column = select_cell1->dynamicCall("Columns").toInt();
        SeriesCollection->setProperty("XValues",QString("=%1!%2:%3").arg(tr("WLP10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));
        int column_excel = select_cell->dynamicCall("Column").toInt();
        select_cell1 = sheet_wlp10->querySubObject("Cells(int,int)",40,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("daily"));
        SeriesCollection->setProperty("Values",QString("=%1!%2:%3").arg(tr("WLP10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        select_cell1 = sheet_wlp10->querySubObject("Cells(int,int)",41,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("accmulate"));
        SeriesCollection->setProperty("Values",QString("=%1!%2:%3").arg(tr("WLP10worstrate")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        //WLP 한도 생산 10대 불량율
        select_cell= sheet_wlp10_limit->querySubObject("Cells(int,int)",27,16000);
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("end(int)",-4159);
        select_cell = select_cell->querySubObject("offset(int,int)",0,1);
        insert_cell = select_cell->querySubObject("EntireColumn()");
        insert_cell->dynamicCall("Insert(int,int)",-4161,0);
        select_cell = select_cell->querySubObject("offset(int,int)",0,-1);
        select_cell->setProperty("Value",temp_month);
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,-1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",1,2);
        select_cell1->dynamicCall("Merge(bool)",true);
        select_cell1 = select_cell->querySubObject("offset(int,int)",1,-1);
        select_cell1 = select_cell1->querySubObject("Columns()");
        select_cell1->setProperty("Hidden",true);

        select_cell1 = select_cell->querySubObject("Offset(int,int)",1,0);
        select_cell1 = select_cell1->querySubObject("resize(int,int)",14,1);
        select_cell2 = select_cell->querySubObject("offset(int,int)",0,32)->querySubObject("Offset(int,int)",1,0)->querySubObject("resize(int,int)",14,1);
        select_cell2->dynamicCall("Copy");
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell2 = select_cell->querySubObject("offset(int,int)",16,0)->querySubObject("end(int)",-4161)->querySubObject("end(int)",-4161);
        select_cell2->dynamicCall("Copy");
        select_cell1 = select_cell->querySubObject("offset(int,int)",16,0);
        select_cell1->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        temp_column1 = select_cell1->dynamicCall("Column").toInt();
        temp_row1 = select_cell1->dynamicCall("Row").toInt();
        select_cell1 = select_cell->querySubObject("end(int)",-4161);
        temp_column2 = select_cell1->dynamicCall("Column").toInt();
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("end(int)",-4121);
        temp_row2 = select_cell1->dynamicCall("Row").toInt();
        column_size = temp_column2-temp_column1+1;
        row_size = temp_row2-temp_row1+1;
//        qDebug()<<"row_size = "<<row_size<<"column_size = "<<column_size;
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        select_cell1 = select_cell1->querySubObject("Resize(int,int)",15,31);
        select_cell1->dynamicCall("ClearContents()");
        select_cell1 = select_cell->querySubObject("offset(int,int)",-1,1);
        select_cell1->setProperty("Value",QString("%1%2").arg(ui->select_data->date().month()).arg(tr("month")));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,1);
        for(int i=1;i<=cal_day;i++){
            select_cell1 = select_cell->querySubObject("offset(int,int)",0,i);
            select_cell1->setProperty("Value",QString("%1%2").arg(i).arg(tr("day")));
        }

        select_cell1 = select_cell->querySubObject("offset(int,int)",0,32);
        select_cell1->setProperty("Value",tr("accmulate"));
        select_cell1 = select_cell->querySubObject("offset(int,int)",0,31);
        chart_object = sheet_wlp10_limit->querySubObject("ChartObjects(int)",1);
        chart_object = chart_object->querySubObject("Chart");
        SeriesCollection = chart_object->querySubObject("SeriesCollection(int)",1);
        chart_excel_start_address = select_cell->dynamicCall("Address").toString();
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        chart_last_column = select_cell1->dynamicCall("Columns").toInt();
        SeriesCollection->setProperty("XValues",QString("='%1'!%2:%3").arg(tr("WLP10worstrate_limit")).arg(chart_excel_start_address).arg(chart_excel_end_address));
        column_excel = select_cell->dynamicCall("Column").toInt();
        select_cell1 = sheet_wlp10_limit->querySubObject("Cells(int,int)",40,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("daily"));
        SeriesCollection->setProperty("Values",QString("='%1'!%2:%3").arg(tr("WLP10worstrate_limit")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        select_cell1 = sheet_wlp10_limit->querySubObject("Cells(int,int)",41,column_excel);
        chart_excel_start_address = select_cell1->dynamicCall("Address").toString();
        select_cell1 = select_cell1->querySubObject("offset(int,int)",0,31);
        chart_excel_end_address = select_cell1->dynamicCall("Address").toString();
        SeriesCollection = chart_object->querySubObject("SeriesCollection(QString)",tr("accmulate"));
        SeriesCollection->setProperty("Values",QString("='%1'!%2:%3").arg(tr("WLP10worstrate_limit")).arg(chart_excel_start_address).arg(chart_excel_end_address));

        //재작업 양산 시트 초기화

        QAxObject *select_cell5 = sheet_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5->dynamicCall("AutoFilter(int)",1);

        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5 =select_cell5->querySubObject("end(int)",-4121);
        int temp_rowcount1 = select_cell5->dynamicCall("Row").toInt();
        int temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",3,1);
        select_cell5 = select_cell5->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell5->dynamicCall("ClearContents");
        select_cell5->dynamicCall("ClearFormats");
        select_cell5 = sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("EntireColumn");
        select_cell5->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));

        //재작업 한도 시트 초기화
        select_cell5 = sheet_b_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5->dynamicCall("AutoFilter(int)",1);
        select_cell5 = sheet_b_rework->querySubObject("Cells(int,int)",2,1);
        select_cell5 =select_cell5->querySubObject("end(int)",-4121);
        temp_rowcount1 = select_cell5->dynamicCall("Row").toInt();
        temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell5 = sheet_b_rework->querySubObject("Cells(int,int)",3,1);
        select_cell5 = select_cell5->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell5->dynamicCall("ClearContents");
        select_cell5->dynamicCall("ClearFormats");
        select_cell5 = sheet_b_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("EntireColumn");
        select_cell5->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));

        //파손 양손 시트 초기화

        QAxObject *select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",2,1);
        select_cell6->dynamicCall("AutoFilter(int)",1);
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",2,1);
        select_cell6 =select_cell6->querySubObject("end(int)",-4121);
        temp_rowcount1 = select_cell6->dynamicCall("Row").toInt();
        temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",3,1);
        select_cell6 = select_cell6->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell6->dynamicCall("ClearContents");
        select_cell6->dynamicCall("ClearFormats");
        select_cell6 = sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("EntireColumn");
        select_cell6->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));
        //파손 한도 시트 초기화
        select_cell6 = sheet_b_destoryhistory->querySubObject("Cells(int,int)",1,1);
        select_cell6->dynamicCall("AutoFilter(int)",1);
        select_cell6 = sheet_b_destoryhistory->querySubObject("Cells(int,int)",2,1);
        select_cell6 =select_cell6->querySubObject("end(int)",-4121);
        temp_rowcount1 = select_cell6->dynamicCall("Row").toInt();
        temp_clear_rowsize = temp_rowcount1 - 1;
        select_cell6 = sheet_b_destoryhistory->querySubObject("Cells(int,int)",3,1);
        select_cell6 = select_cell6->querySubObject("Resize(int,int)",temp_clear_rowsize,10);
        select_cell6->dynamicCall("ClearContents");
        select_cell6->dynamicCall("ClearFormats");
        select_cell6 = sheet_b_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("EntireColumn");
        select_cell6->setProperty("NumberFormat",QString("m%1 d%2").arg(tr("month")).arg(tr("day")));


        //외관검사불량율

        QAxObject *select_cell7 = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,2);
        QAxObject *select_cell8;
        QAxObject *select_cell9;
        QAxObject *select_cell10;
        int i = 1;
        int temp_data_column_count = 0;
        while(true){
            select_cell8 =select_cell7->querySubObject("offset(int,int)",0,i);
            QString value = select_cell8->dynamicCall("Value").toString();
            if(value.indexOf(tr("month"))<0){
                temp_data_column_count = i;
                break;
            }
            i++;
        }
        select_cell9 = select_cell8->querySubObject("EntireColumn");
        select_cell9->dynamicCall("Insert(int,int)",-4161,0);
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-1);
        select_cell9->setProperty("Value",QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month")));
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-2)->querySubObject("EntireColumn");
        select_cell9->setProperty("Hidden",true);

        select_cell9  = select_cell8->querySubObject("offset(int,int)",1,31);
        select_cell9 = select_cell9->querySubObject("resize(int,int)",6,1);
        select_cell9->dynamicCall("copy");

        select_cell10 = select_cell8->querySubObject("offset(int,int)",1,-1);
        select_cell10->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell9 = select_cell8->querySubObject("resize(int,int)",5,31);
        select_cell9->dynamicCall("ClearContents");
        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            select_cell9->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i).toString("yyyy-MM-dd")));
        }

        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            int today =  ui->select_data->date().day();
            int search_day = select_cell9->dynamicCall("Value").toDate().day();
            if(search_day<=10){
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",false);
            }else {
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",true);
            }
        }

        //한도 생산 외관검사불량율
        select_cell7 = sheet_limitproductvaild->querySubObject("Cells(int,int)",17,2);
        i = 1;
        temp_data_column_count = 0;
        while(true){
            select_cell8 =select_cell7->querySubObject("offset(int,int)",0,i);
            QString value = select_cell8->dynamicCall("Value").toString();
            if(value.indexOf(tr("month"))<0){
                temp_data_column_count = i;
                break;
            }
            i++;
        }
        select_cell9 = select_cell8->querySubObject("EntireColumn");
        select_cell9->dynamicCall("Insert(int,int)",-4161,0);
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-1);
        select_cell9->setProperty("Value",QString("%1%2").arg(ui->select_data->date().addMonths(-1).month()).arg(tr("month")));
        select_cell9 = select_cell8->querySubObject("offset(int,int)",0,-2)->querySubObject("EntireColumn");
        select_cell9->setProperty("Hidden",true);


        select_cell9  = select_cell8->querySubObject("offset(int,int)",1,31);
        select_cell9 = select_cell9->querySubObject("resize(int,int)",5,1);
        select_cell9->dynamicCall("copy");

        select_cell10 = select_cell8->querySubObject("offset(int,int)",1,-1);
        select_cell10->dynamicCall("PasteSpecial(int,int,bool,bool)",-4163,-4142,false,false);

        select_cell9 = select_cell8->querySubObject("resize(int,int)",6,31);
        select_cell9->dynamicCall("ClearContents");
        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            select_cell9->setProperty("Value",QString("%1").arg(QDate(ui->select_data->date().year(),ui->select_data->date().month(),i).toString("yyyy-MM-dd")));
        }

        for(int i=1;i<=cal_day;i++){
            select_cell9 = select_cell8->querySubObject("offset(int,int)",0,i-1);
            int today =  ui->select_data->date().day();
            int search_day = select_cell9->dynamicCall("Value").toDate().day();
            if(search_day<=10){
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",false);
            }else {
                select_cell9 = select_cell9->querySubObject("EntireColumn");
                select_cell9->setProperty("Hidden",true);
            }
        }

    }

    //WLP 10대불량율

    QAxObject *modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",26,2);

    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    QString month_str = modify_cell->dynamicCall("Value()").toString().split(tr("month")).at(0);
    int write_month = month_str.toInt();
    if(write_month = ui->select_data->date().month()){
        qDebug()<<"match month";
        qDebug()<<write_month;
    }

    int i=2;
    int cellcolumn = 0;
    while(true){
        modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",27,i);
        QString excel_day = modify_cell->dynamicCall("Value()").toString();

        QString user_day = QString("%1%2").arg(ui->select_data->date().day()).arg(tr("day"));
        if(excel_day == user_day){
            cellcolumn = modify_cell->dynamicCall("Column()").toInt();
            qDebug()<<"select day cell cellcolumn = "<<cellcolumn;
            break;
        }
        i++;
    }
    double daily_total_sum = 0.0;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP003_worst());
    daily_total_sum += th->getTotal_DP003_worst();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP004_worst());
    daily_total_sum += th->getTotal_DP004_worst();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP006_worst());
    daily_total_sum += th->getTotal_DP006_worst();

    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP005_worst());
    daily_total_sum += th->getTotal_DP005_worst();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP008_worst());
    daily_total_sum += th->getTotal_DP008_worst();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP001_worst());
    daily_total_sum += th->getTotal_DP001_worst();
    double totalmachine_fail = th->getTotal_Machinefail_defect()+th->getTotal_Machinefail_eatching()
                                +th->getTotal_Machinefail_light()+th->getTotal_Machinefail_probe();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",totalmachine_fail);
    daily_total_sum += totalmachine_fail;
    double totalworker_fail = th->getTotal_Workerfail_defect()+th->getTotal_Workerfail_light()+th->getTotal_Workerfail_eatching()
                                +th->getTotal_Workerfail_output()+th->getTotal_Workerfail_probe();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",totalworker_fail);
    daily_total_sum += totalworker_fail;
//    double total_prcdspecout = th->getTotal_prcdlow()+th->getTotal_prcdhigh();
//    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",36,cellcolumn);
//    modify_cell->setProperty("Value",total_prcdspecout);
//    daily_total_sum += total_prcdspecout;
//    double total_metalspecout = th->getTotal_metallow()+th->getTotal_metalhigh();
//    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",37,cellcolumn);
//    modify_cell->setProperty("Value",total_metalspecout);
//    daily_total_sum += total_metalspecout;
    double totalpaticle= th->getTotal_Defectpaticle()+th->getTotal_Etcpaticle()
                            +th->getTotal_Padpaticle()+th->getTotal_Pattenpaticle()+th->getTotal_Rework_paticle();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",totalpaticle);
    daily_total_sum += totalpaticle;
    double totaljobmiss = th->getTotal_Jobmiss_defect()+th->getTotal_Jobmiss_eatching()
                            +th->getTotal_Jobmiss_light()+th->getTotal_Jobmiss_probe();
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",totaljobmiss);
    daily_total_sum += totaljobmiss;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",daily_total_sum);
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",th->getLast_probe_vild_daily());
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",40,cellcolumn);
    modify_cell->setProperty("Value",th->getDaily_totalvild());
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getAccumulate_totalvild());

    modify_cell = modify_cell->querySubObject("EntireColumn");
    modify_cell->setProperty("Hidden",false);

    //누적

    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    cellcolumn = modify_cell->dynamicCall("Column()").toInt();
    double accumulate_daily_total_sum = 0.0;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP003_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP003_worst;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP004_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP004_worst;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP006_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP006_worst;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP005_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP005_worst;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP008_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP008_worst;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_DP001_worst);
    accumulate_daily_total_sum += th->accumulate_total_DP001_worst;
    double accumulate_totalmachine_fail = th->accumulate_total_Machinefail_defect+th->accumulate_total_Machinefail_eatching
                                +th->accumulate_total_Machinefail_light+th->accumulate_total_Machinefail_probe;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalmachine_fail);
    accumulate_daily_total_sum += accumulate_totalmachine_fail;
    double accumulate_totalworker_fail = th->accumulate_total_Workerfail_defect+th->accumulate_total_Workerfail_light+th->accumulate_total_Workerfail_eatching
                                +th->accumulate_total_Workerfail_output+th->accumulate_total_Workerfail_probe;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalworker_fail);
    accumulate_daily_total_sum += accumulate_totalworker_fail;
    double accumulate_totalpaticle= th->accumulate_total_Defectpaticle+th->accumulate_total_Etcpaticle
                            +th->accumulate_total_Padpaticle+th->accumulate_total_Pattenpaticle+th->accumulate_total_Rework_paticle;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totalpaticle);
    accumulate_daily_total_sum += accumulate_totalpaticle;
    double accumulate_totaljobmiss = th->accumulate_total_Jobmiss_defect+th->accumulate_total_Jobmiss_eatching
                            +th->accumulate_total_Jobmiss_light+th->accumulate_total_Jobmiss_probe;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",accumulate_totaljobmiss);
    accumulate_daily_total_sum += accumulate_totaljobmiss;
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",accumulate_daily_total_sum);
    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_last_probe_vild_daily);

    modify_cell = sheet_wlp10->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getAccumulate_totalvild());

    modify_cell = modify_cell->querySubObject("EntireColumn");
    modify_cell->setProperty("Hidden",false);

    //WLP 한도생산 10대불량율

    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",26,2);

    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    month_str = modify_cell->dynamicCall("Value()").toString().split(tr("month")).at(0);
    write_month = month_str.toInt();
    if(write_month = ui->select_data->date().month()){
        qDebug()<<"match month";
        qDebug()<<write_month;
    }

    i=2;
    cellcolumn = 0;
    while(true){
        modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",27,i);
        QString excel_day = modify_cell->dynamicCall("Value()").toString();

        QString user_day = QString("%1%2").arg(ui->select_data->date().day()).arg(tr("day"));
        if(excel_day == user_day){
            cellcolumn = modify_cell->dynamicCall("Column()").toInt();
            qDebug()<<"select day cell cellcolumn = "<<cellcolumn;
            break;
        }
        i++;
    }
    daily_total_sum = 0;
    totalmachine_fail = 0;
    totalworker_fail = 0;
    totalpaticle =0;
    totaljobmiss=0;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP003_worst());
    daily_total_sum += th->getTotal_b_DP003_worst();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP004_worst());
    daily_total_sum += th->getTotal_b_DP004_worst();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP006_worst());
    daily_total_sum += th->getTotal_b_DP006_worst();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP005_worst());
    daily_total_sum += th->getTotal_b_DP005_worst();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP008_worst());
    daily_total_sum += th->getTotal_b_DP008_worst();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_b_DP001_worst());
    daily_total_sum += th->getTotal_b_DP001_worst();

    totalmachine_fail = th->getTotal_b_Machinefail_defect()+th->getTotal_b_Machinefail_eatching()
                                +th->getTotal_b_Machinefail_light()+th->getTotal_b_Machinefail_probe();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",totalmachine_fail);
    daily_total_sum += totalmachine_fail;
    totalworker_fail = th->getTotal_b_Workerfail_defect()+th->getTotal_b_Workerfail_light()+th->getTotal_b_Workerfail_eatching()
                                +th->getTotal_b_Workerfail_output()+th->getTotal_b_Workerfail_probe();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",totalworker_fail);
    daily_total_sum += totalworker_fail;
//    double total_prcdspecout = th->getTotal_prcdlow()+th->getTotal_prcdhigh();
//    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",36,cellcolumn);
//    modify_cell->setProperty("Value",total_prcdspecout);
//    daily_total_sum += total_prcdspecout;
//    double total_metalspecout = th->getTotal_metallow()+th->getTotal_metalhigh();
//    modify_cell = sheet_fab10->querySubObject("Cells(int,int)",37,cellcolumn);
//    modify_cell->setProperty("Value",total_metalspecout);
//    daily_total_sum += total_metalspecout;
    totalpaticle= th->getTotal_Defectpaticle()+th->getTotal_b_Etcpaticle()
                            +th->getTotal_b_Padpaticle()+th->getTotal_b_Pattenpaticle()+th->getTotal_b_Rework_paticle();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",totalpaticle);
    daily_total_sum += totalpaticle;
    totaljobmiss = th->getTotal_Jobmiss_defect()+th->getTotal_b_Jobmiss_eatching()
                            +th->getTotal_b_Jobmiss_light()+th->getTotal_b_Jobmiss_probe();
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",totaljobmiss);
    daily_total_sum += totaljobmiss;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",daily_total_sum);
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",th->getB_last_probe_vild_daily());
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",40,cellcolumn);
    modify_cell->setProperty("Value",th->getB_daily_totalvild());
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getB_accumulate_totalvild());

    //누적

    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell = modify_cell->querySubObject("end(int)",-4161);
    cellcolumn = modify_cell->dynamicCall("Column()").toInt();
    double accumulate_b_daily_total_sum = 0.0;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP003_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP003_worst;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP004_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP004_worst;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP006_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP006_worst;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP005_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP005_worst;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP008_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP008_worst;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_total_b_DP001_worst);
    accumulate_b_daily_total_sum += th->accumulate_total_b_DP001_worst;
    double accumulate_b_totalmachine_fail = th->accumulate_total_b_Machinefail_defect+th->accumulate_total_b_Machinefail_eatching
                                +th->accumulate_total_b_Machinefail_light+th->accumulate_total_b_Machinefail_probe;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",accumulate_b_totalmachine_fail);
    accumulate_b_daily_total_sum += accumulate_b_totalmachine_fail;
    double accumulate_b_totalworker_fail = th->accumulate_total_b_Workerfail_defect+th->accumulate_total_b_Workerfail_light+th->accumulate_total_b_Workerfail_eatching
                                +th->accumulate_total_b_Workerfail_output+th->accumulate_total_b_Workerfail_probe;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",accumulate_b_totalworker_fail);
    accumulate_b_daily_total_sum += accumulate_b_totalworker_fail;
    double accumulate_b_totalpaticle= th->accumulate_total_Defectpaticle+th->accumulate_total_Etcpaticle
                            +th->accumulate_total_Padpaticle+th->accumulate_total_Pattenpaticle+th->accumulate_total_Rework_paticle;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",accumulate_b_totalpaticle);
    accumulate_b_daily_total_sum += accumulate_b_totalpaticle;
    double accumulate_b_totaljobmiss = th->accumulate_total_Jobmiss_defect+th->accumulate_total_Jobmiss_eatching
                            +th->accumulate_total_Jobmiss_light+th->accumulate_total_Jobmiss_probe;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",accumulate_b_totaljobmiss);
    accumulate_b_daily_total_sum += accumulate_b_totaljobmiss;
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",accumulate_b_daily_total_sum);
    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",th->accumulate_b_last_probe_vild_daily);

    modify_cell = sheet_wlp10_limit->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getB_accumulate_totalvild());



    //외관 검사 불량률

    modify_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,2);
    i = 2;
    while(true){
        modify_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",17,i);
        QString exceldate = modify_cell->dynamicCall("Value()").toDate().toString("yyyy-MM-dd");
        if(exceldate == ui->select_data->date().toString("yyyy-MM-dd")){
            break;
        }
        i++;
    }
    int exteriorcheck_colum = i;

    QAxObject *exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",18,exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getExterior_pattenpaticle_vaild());
    exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",19,exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getExterior_padpaticle_vaild());
    exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",20,exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getExterior_sindiseatching_vaild());
    exteriorcheck_cell = sheet_exteriorcheck->querySubObject("Cells(int,int)",21,exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getExterior_etcpaticle_vaild());

    exteriorcheck_cell = exteriorcheck_cell->querySubObject("EntireColumn");
    exteriorcheck_cell->setProperty("Hidden",false);

    //한도생산외관검사불량율

    modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",17,2);
    i = 2;
    while(true){
        modify_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",17,i);
        QString exceldate = modify_cell->dynamicCall("Value()").toDate().toString("yyyy-MM-dd");
        if(exceldate == ui->select_data->date().toString("yyyy-MM-dd")){
            break;
        }
        i++;
    }
    int b_exteriorcheck_colum = i;

    exteriorcheck_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",18,b_exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getB_exterior_pattenpaticle_vaild());
    exteriorcheck_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",19,b_exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getB_exterior_padpaticle_vaild());
    exteriorcheck_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",20,b_exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getB_exterior_sindiseatching_vaild());
    exteriorcheck_cell = sheet_limitproductvaild->querySubObject("Cells(int,int)",21,b_exteriorcheck_colum);
    exteriorcheck_cell->setProperty("Value",th->getB_exterior_etcpaticle_vaild());

    exteriorcheck_cell = exteriorcheck_cell->querySubObject("EntireColumn");
    exteriorcheck_cell->setProperty("Hidden",false);

    //재작업이력_양산

    modify_cell =  sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
    modify_cell->dynamicCall("AutoFilter(int)",1);

    modify_cell = sheet_rework->querySubObject("Cells(int,int)",3,1);
    modify_cell = modify_cell->querySubObject("end(int)",-4121);
    int rework_write_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
    if(ui->select_data->date().day()==1){
        rework_write_point_row=3;
    }
    if(rework_write_point_row > 10000){
        rework_write_point_row=3;
    }

    for(int j=0;j<th->getRework_worstcpsprocesslist().count();j++){
        rework_text_type_WLP temp_type = th->getRework_worstcpsprocesslist().at(j);
        QAxObject *rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,1);
        rework_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));
        double temp_data1 = 100.0-(double)th->getDaily_totalvild();
        double temp_data2 = (double)temp_type.getChip_count()/(double)th->getWorst_sum();
        qDebug()<<(double)th->getWorst_sum();
        double worst_rate = temp_data2 *temp_data1;

        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,2);
        rework_write_cell->setProperty("Value",RoundOff(worst_rate,2));
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,3);
        rework_write_cell->setProperty("Value",temp_type.getProduct_name());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,4);
        rework_write_cell->setProperty("Value",temp_type.getLOT());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,5);
        rework_write_cell->setProperty("Value",temp_type.getProcess());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,6);
        rework_write_cell->setProperty("Value",temp_type.getLot_count());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,7);
        rework_write_cell->setProperty("Value",temp_type.getChip_count());
        rework_write_cell = sheet_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,8);
        rework_write_cell->setProperty("Value",temp_type.getContent());
    }
     modify_cell =  sheet_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     QString selection_date = QString("%1%2 %3%4").arg(ui->select_data->date().month()).arg(tr("month"))
                                                  .arg(ui->select_data->date().day()).arg(tr("day"));

     modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);

     //재작업이력_한도

     modify_cell =  sheet_b_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     modify_cell->dynamicCall("AutoFilter(int)",1);

     modify_cell = sheet_b_rework->querySubObject("Cells(int,int)",3,1);
     modify_cell = modify_cell->querySubObject("end(int)",-4121);
     rework_write_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
     if(ui->select_data->date().day()==1){
         rework_write_point_row=3;
     }
     if(rework_write_point_row > 10000){
         rework_write_point_row=3;
     }

     for(int j=0;j<th->getB_rework_worstcpsprocesslist().count();j++){
         rework_text_type_WLP temp_type = th->getB_rework_worstcpsprocesslist().at(j);
         QAxObject *rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,1);
         rework_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));
         double temp_data1 = 100.0-(double)th->getB_daily_totalvild();
         double temp_data2 = (double)temp_type.getChip_count()/(double)th->getB_worst_sum();
         double worst_rate = temp_data2 *temp_data1;

         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,2);
         rework_write_cell->setProperty("Value",RoundOff(worst_rate,2));
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,3);
         rework_write_cell->setProperty("Value",temp_type.getProduct_name());
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,4);
         rework_write_cell->setProperty("Value",temp_type.getLOT());
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,5);
         rework_write_cell->setProperty("Value",temp_type.getProcess());
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,6);
         rework_write_cell->setProperty("Value",temp_type.getLot_count());
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,7);
         rework_write_cell->setProperty("Value",temp_type.getChip_count());
         rework_write_cell = sheet_b_rework->querySubObject("Cells(int,int)",rework_write_point_row+j,8);
         rework_write_cell->setProperty("Value",temp_type.getContent());
     }
      modify_cell =  sheet_b_rework->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
      selection_date = QString("%1%2 %3%4").arg(ui->select_data->date().month()).arg(tr("month"))
                                                   .arg(ui->select_data->date().day()).arg(tr("day"));

      modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);

     //파손 양산

     modify_cell =  sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     modify_cell->dynamicCall("AutoFilter(int)",1);

     modify_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",1,1);
     modify_cell = modify_cell->querySubObject("end(int)",-4121);
     int distoryhistory_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
     if(ui->select_data->date().day()==1){
         distoryhistory_point_row=3;
     }
     if(distoryhistory_point_row>10000){
         distoryhistory_point_row=3;
     }
     for(int j=0;j<th->getDefect_worstcpsprocesslist().count();j++){
         defect_worst_type_WLP defect_item;
         defect_item = th->getDefect_worstcpsprocesslist().at(j);
         QAxObject *distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,1);
         distoryhistory_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));

         double temp_data1 = 100.0-(double)th->getDaily_totalvild();
         double temp_data2 = (double)defect_item.getChip_count()/(double)th->getWorst_sum();

         double worst_rate =  temp_data2 *temp_data1;
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,2);
         distoryhistory_write_cell->setProperty("Value",RoundOff(worst_rate,2));
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,3);
         distoryhistory_write_cell->setProperty("Value",defect_item.getProduct_name());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,4);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLOT());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,5);
         distoryhistory_write_cell->setProperty("Value",defect_item.getDefect_name());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,6);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLot_count());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,7);
         distoryhistory_write_cell->setProperty("Value",defect_item.getChip_count());
//         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,8);
//         distoryhistory_write_cell->setProperty("Value",defect_item.getMetarial_grouptype());
         distoryhistory_write_cell = sheet_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,8);
         distoryhistory_write_cell->setProperty("Value",defect_item.getContent());
     }
     modify_cell =  sheet_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",215,10);

     modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);

     //파손 한도

     modify_cell =  sheet_b_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",193,15);
     modify_cell->dynamicCall("AutoFilter(int)",1);

     modify_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",1,1);
     modify_cell = modify_cell->querySubObject("end(int)",-4121);
     distoryhistory_point_row = modify_cell->dynamicCall("Row()").toInt()+1;
     if(ui->select_data->date().day()==1){
         distoryhistory_point_row=3;
     }
     if(distoryhistory_point_row>10000){
         distoryhistory_point_row=3;
     }
     for(int j=0;j<th->getB_defect_worstcpsprocesslist().count();j++){
         defect_worst_type_WLP defect_item;
         defect_item = th->getB_defect_worstcpsprocesslist().at(j);
         QAxObject *distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,1);
         distoryhistory_write_cell->setProperty("Value",ui->select_data->date().toString("yyyy-MM-dd"));

         double temp_data1 = 100.0-(double)th->getB_daily_totalvild();
         double temp_data2 = (double)defect_item.getChip_count()/(double)th->getB_worst_sum();

         double worst_rate =  temp_data2 *temp_data1;
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,2);
         distoryhistory_write_cell->setProperty("Value",RoundOff(worst_rate,2));
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,3);
         distoryhistory_write_cell->setProperty("Value",defect_item.getProduct_name());
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,4);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLOT());
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,5);
         distoryhistory_write_cell->setProperty("Value",defect_item.getDefect_name());
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,6);
         distoryhistory_write_cell->setProperty("Value",defect_item.getLot_count());
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,7);
         distoryhistory_write_cell->setProperty("Value",defect_item.getChip_count());
//         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,8);
//         distoryhistory_write_cell->setProperty("Value",defect_item.getMetarial_grouptype());
         distoryhistory_write_cell = sheet_b_destoryhistory->querySubObject("Cells(int,int)",distoryhistory_point_row+j,8);
         distoryhistory_write_cell->setProperty("Value",defect_item.getContent());
     }
     modify_cell =  sheet_b_destoryhistory->querySubObject("Cells(int,int)",1,1)->querySubObject("resize(int,int)",215,10);

     modify_cell->dynamicCall("AutoFilter(int,QString,int)",1,selection_date,1);

     workbook->dynamicCall("Save");
     excel->dynamicCall("Quit()");



}

void worst_search_excel_main::on_fileselect_dialog_btn_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("open file"),
                                                    qApp->applicationFilePath(),
                                                    tr("excel (*.xlsx)"));
    QString configini_str = qApp->applicationDirPath()+"/worst_excel_setting.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setting");
    settings.setValue("worst_excel_file",filepath);
    settings.endGroup();
    ui->LE_filepath->setText(filepath);
}

void worst_search_excel_main::slot_debug_output(QString str)
{
    ui->debug_text->append(QString("%1 \n").arg(str));
}

void worst_search_excel_main::slot_debug_output_WLP(QString str)
{
    ui->debug_WLP_text->append(QString("%1 \n").arg(str));
}

void worst_search_excel_main::on_fileselect_WLP_dialog_btn_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("open file"),
                                                    qApp->applicationFilePath(),
                                                    tr("excel (*.xlsx)"));
    QString configini_str = qApp->applicationDirPath()+"/worst_excel_setting.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setting");
    settings.setValue("worst_WLP_excel_file",filepath);
    settings.endGroup();
    ui->LE_WLP_filepath->setText(filepath);
}

void worst_search_excel_main::on_CSP_Process_add_btn_clicked()
{
    CSP_model->insertRow(CSP_model->rowCount());
}

void worst_search_excel_main::on_CSP_Process_del_btn_clicked()
{
    if(ui->CSP_Process->selectionModel()->selectedIndexes().count()>0){
        qDebug()<<ui->CSP_Process->selectionModel()->selectedIndexes().first().row();
        CSP_model->removeRow(ui->CSP_Process->selectionModel()->selectedIndexes().first().row());
        CSP_model->select();
    }
}

void worst_search_excel_main::on_WLP_Process_add_btn_clicked()
{
    WLP_model->insertRow(WLP_model->rowCount());
}

void worst_search_excel_main::on_WLP_Process_del_btn_clicked()
{
    if(ui->WLP_Process->selectionModel()->selectedIndexes().count()>0){
        qDebug()<<ui->WLP_Process->selectionModel()->selectedIndexes().first().row();
        WLP_model->removeRow(ui->WLP_Process->selectionModel()->selectedIndexes().first().row());
        WLP_model->select();
    }
}

void worst_search_excel_main::on_work_plan_add_btn_clicked()
{
    QSqlQuery litequery(litedb);
    litequery.exec(QString("insert into work_plan(plan_time) values('%1')").arg(ui->work_plan_timeedit->time().toString("hh:mm:ss")));
    work_plan_model->select();
}

void worst_search_excel_main::work_plan_timer_timeout()
{
    QSqlQuery litequery(litedb);
    litequery.exec(QString("select * from work_plan where plan_time = '%1'").arg(QTime::currentTime().toString("hh:mm:ss")));
    if(litequery.next()){
        work_plan_timer.setInterval(2000);
        ui->select_data->setDate(QDate::currentDate().addDays(-1));
        on_start_btn_clicked();
    }else {
        work_plan_timer.setInterval(300);
    }
}

void worst_search_excel_main::on_work_plan_del_btn_clicked()
{
    if(ui->work_plan_table->selectionModel()->selectedIndexes().count()>0){
        qDebug()<<ui->work_plan_table->selectionModel()->selectedIndexes().first().row();
        work_plan_model->removeRow(ui->work_plan_table->selectionModel()->selectedIndexes().first().row());
        work_plan_model->select();
    }
}

void worst_search_excel_main::on_type_price_add_btn_clicked()
{
    type_price_model->insertRow(type_price_model->rowCount());
}

void worst_search_excel_main::on_type_price_del_btn_clicked()
{
    if(ui->type_price_table->selectionModel()->selectedIndexes().count()>0){
        qDebug()<<ui->type_price_table->selectionModel()->selectedIndexes().first().row();
        type_price_model->removeRow(ui->type_price_table->selectionModel()->selectedIndexes().first().row());
        type_price_model->select();
    }
}
