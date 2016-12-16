#include "worst_search_excel_main.h"
#include "ui_worst_search_excel_main.h"

worst_search_excel_main::worst_search_excel_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::worst_search_excel_main)
{
    ui->setupUi(this);
    QString configini_str = qApp->applicationDirPath()+"/worst_excel_setting.ini";
    QString filepath;
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setting");
    filepath = settings.value("worst_excel_file").toString();
    settings.endGroup();
    ui->LE_filepath->setText(filepath);
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
    th->start();
    connect(th,SIGNAL(sig_excel_work()),this,SLOT(slot_excel_work()));
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
    QAxObject *sheet = workbook->querySubObject( "Worksheets(QString)",tr("FAB10worstrate"));

    QAxObject *modify_cell = sheet->querySubObject("Cells(int,int)",26,2);

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
        modify_cell = sheet->querySubObject("Cells(int,int)",27,i);
        QString excel_day = modify_cell->dynamicCall("Value()").toString();

        QString user_day = QString("%1%2").arg(ui->select_data->date().day()).arg(tr("day"));
        if(excel_day == user_day){
            cellcolumn = modify_cell->dynamicCall("Column()").toInt();
            qDebug()<<"select day cell cellcolumn = "<<cellcolumn;
            break;
        }
        i++;
    }
    modify_cell = sheet->querySubObject("Cells(int,int)",28,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP003_worst());
    modify_cell = sheet->querySubObject("Cells(int,int)",29,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP004_worst());
    modify_cell = sheet->querySubObject("Cells(int,int)",30,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP006_worst());
    modify_cell = sheet->querySubObject("Cells(int,int)",31,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP001_worst());
    modify_cell = sheet->querySubObject("Cells(int,int)",32,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP005_worst());
    modify_cell = sheet->querySubObject("Cells(int,int)",33,cellcolumn);
    modify_cell->setProperty("Value",th->getTotal_DP008_worst());
    double totalmachine_fail = th->getTotal_Machinefail_defect()+th->getTotal_Machinefail_eatching()
                                +th->getTotal_Machinefail_light()+th->getTotal_Machinefail_probe();
    modify_cell = sheet->querySubObject("Cells(int,int)",34,cellcolumn);
    modify_cell->setProperty("Value",totalmachine_fail);
    double totalworker_fail = th->getTotal_Workerfail_defect()+th->getTotal_Workerfail_light()+th->getTotal_Workerfail_eatching()
                                +th->getTotal_Workerfail_output()+th->getTotal_Workerfail_probe();
    modify_cell = sheet->querySubObject("Cells(int,int)",35,cellcolumn);
    modify_cell->setProperty("Value",totalworker_fail);
    double total_prcdspecout = th->getTotal_prcdlow()+th->getTotal_prcdhigh();
    modify_cell = sheet->querySubObject("Cells(int,int)",36,cellcolumn);
    modify_cell->setProperty("Value",total_prcdspecout);
    double total_metalspecout = th->getTotal_metallow()+th->getTotal_metalhigh();
    modify_cell = sheet->querySubObject("Cells(int,int)",37,cellcolumn);
    modify_cell->setProperty("Value",total_metalspecout);
    double totalpaticle= th->getTotal_Defectpaticle()+th->getTotal_Etcpaticle()
                            +th->getTotal_Padpaticle()+th->getTotal_Pattenpaticle();
    modify_cell = sheet->querySubObject("Cells(int,int)",38,cellcolumn);
    modify_cell->setProperty("Value",totalpaticle);
    double totaljobmiss = th->getTotal_Jobmiss_defect()+th->getTotal_Jobmiss_eatching()
                            +th->getTotal_Jobmiss_light()+th->getTotal_Jobmiss_probe();
    modify_cell = sheet->querySubObject("Cells(int,int)",39,cellcolumn);
    modify_cell->setProperty("Value",totaljobmiss);
    modify_cell = sheet->querySubObject("Cells(int,int)",41,cellcolumn);
    modify_cell->setProperty("Value",th->getLast_probe_vild_daily());
    modify_cell = sheet->querySubObject("Cells(int,int)",42,cellcolumn);
    modify_cell->setProperty("Value",th->getDaily_totalvild());
    modify_cell = sheet->querySubObject("Cells(int,int)",43,cellcolumn);
    modify_cell->setProperty("Value",th->getAccumulate_totalvild());

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

}
