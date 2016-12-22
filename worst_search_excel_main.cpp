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
}

worst_search_excel_main::~worst_search_excel_main()
{
    delete ui;
}

void worst_search_excel_main::on_start_btn_clicked()
{

    worst_search_th *th = new worst_search_th();
    th->setSelect_date(ui->select_data->date());
//    th->start();
    connect(th,SIGNAL(sig_excel_work()),this,SLOT(slot_excel_work()));
    connect(th,SIGNAL(sig_debug_output(QString)),this,SLOT(slot_debug_output(QString)));
    worst_search_WLP_th *th_WLP = new worst_search_WLP_th();
    th_WLP->setSelect_date(ui->select_data->date());
    th_WLP->start();
    connect(th_WLP,SIGNAL(sig_debug_output(QString)),this,SLOT(slot_debug_output_WLP(QString)));
    connect(th_WLP,SIGNAL(sig_excel_work()),this,SLOT(slot_excel_WLP_work()));


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
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "from",
                                                                     tr("1day continue because remove ago monuth data marge and remove?\n"),
                                                                     QMessageBox::Cancel| QMessageBox::Yes);
        if(resBtn != QMessageBox::Yes){
            return;
        }
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
            if(search_day<=today){
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
                            +th->getTotal_Padpaticle()+th->getTotal_Pattenpaticle();
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
         distoryhistory_write_cell->setProperty("Value",defect_item.getMetarial_grouptype());
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


}

void worst_search_excel_main::slot_excel_WLP_work()
{

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
