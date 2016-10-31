#include "eismain.h"
#include "ui_eismain.h"

EISmain::EISmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EISmain)
{
    ui->setupUi(this);
    time_update.setInterval(1000);
    time_update.start();
    connect(&time_update,SIGNAL(timeout()),this,SLOT(time_update_out()));
    QString configini_str = "server.ini";
    ftp = new QFtp(this);

    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    server_ip = settings.value("serverip").toString();
    db_port = settings.value("dbport").toString();\
    ftp_port = settings.value("ftpport").toString();
    settings.endGroup();

    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));


    if(!db.contains("EISDB")){
       db = QSqlDatabase::addDatabase("QMYSQL","EISDB");
       db.setHostName(server_ip);
       db.setPort(db_port.toInt());
       db.setUserName("fabuser");
       db.setPassword("wisolfab");
       db.setDatabaseName("FAB");
    }else {
       db = QSqlDatabase::database("EISDB");
    }



    if(!db.open()){
         int ret = QMessageBox::warning(this, tr("conntion false"),
                                           "server connection fail\n"
                                              ""+db.lastError().text(),
                                                QMessageBox::Close);
    }

    //docnumber을 +1 한값을 데이터베이스 업로드 시켜 문서가 서로 중첩되지 않게한다.
    QSqlQuery query(db);

    QString str_query = QString("update EIS_management set document_number = EIS_management.document_number +1;");
    query.exec(str_query);

    query.exec("select * from EIS_management");
    query.next();
    doc_number = query.value("document_number").toInt();

    query.exec("select team from People_information GROUP by team;");
    while(query.next()){
        ui->select_team->addItem(query.value("team").toString());
    }
    QHeaderView *tempheader;
    tempheader = ui->tableWidget_2->horizontalHeader();
    connect(tempheader,SIGNAL(sectionClicked(int)),this,SLOT(header_click(int)));
    phenomenon_edit = new BTextEdit(&doc_number,this);
    ui->layout_phenomenon->addWidget(phenomenon_edit,0,0);
    cause_edit = new BTextEdit(&doc_number,this);
    ui->layout_cause->addWidget(cause_edit,0,0);
    current_action_edit = new BTextEdit(&doc_number,this);
    ui->layout_current_action->addWidget(current_action_edit,0,0);
    current_lot_action_edit = new BTextEdit(&doc_number,this);
    ui->layout_current_lot_action->addWidget(current_lot_action_edit,0,0);
    change_master_sheet_edit = new BTextEdit(&doc_number,this);
    ui->layout_change_master_sheet->addWidget(change_master_sheet_edit,0,0);
    next_shift_edit = new BTextEdit(&doc_number,this);
    ui->layout_next_shift->addWidget(next_shift_edit,0,0);
    part_change_edit = new BTextEdit(&doc_number,this);
    ui->layout_part_change->addWidget(part_change_edit,0,0);
    after_lot_monitering_edit = new BTextEdit(&doc_number,this);
    ui->layout_after_lot_monitering->addWidget(after_lot_monitering_edit,0,0);

}

EISmain::~EISmain()
{
    delete ui;
}

void EISmain::on_tableWidget_2_clicked(const QModelIndex &index)
{
    qDebug()<<"test";
}

void EISmain::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    qDebug()<<"test";
}
void EISmain::header_click(int data){
    qDebug()<<"test";
}

void EISmain::time_update_out()
{
    ui->current_time->setDateTime(QDateTime::currentDateTime());
}

void EISmain::on_select_team_currentIndexChanged(const QString &arg1)
{
    ui->select_process->clear();;
    ui->select_process->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_process->addItem(query.value("factory_process").toString());
    }
}



void EISmain::on_select_process_currentIndexChanged(const QString &arg1)
{
    ui->select_facilities->clear();;
    ui->select_facilities->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }
}

void EISmain::on_select_facilities_currentIndexChanged(const QString &arg1)
{
    ui->select_name->clear();;
    ui->select_name->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select name from People_information where facilities = '%1'").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_name->addItem(query.value("name").toString());
    }
}

void EISmain::on_add_button_clicked()
{
    QString now_datetime =ui->current_time->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString change_have;
    if(ui->radio_btn_change_exist->isChecked()){
        change_have = QString("%1").arg(1);
    }else {
        change_have = QString("%1").arg(0);
    }
    QSqlQuery query(db);
    QString Time_part = '1';
    QTime nowtime = ui->current_time->time();
    int second_diff = nowtime.secsTo(QTime(17,00,0));
    if(second_diff>=0){
        Time_part = "1";
    }else{
        Time_part = "0";
    }
    QString document_name = ui->document_name->text();
    QString team_name = ui->select_team->currentText();
    QString process = ui->select_process->currentText();
    QString write_name = ui->select_name->currentText();
    QString facilities_name = ui->select_facilities->currentText();
    QString phenomenon = phenomenon_edit->tosqlhtml();
    QString cause = cause_edit->tosqlhtml();
    QString current_action = current_action_edit->tosqlhtml();
    QString current_lot_action = current_lot_action_edit->tosqlhtml();
    QString change_master_sheet = change_master_sheet_edit->tosqlhtml();
    QString after_lot_monitering = after_lot_monitering_edit->tosqlhtml();
    QString next_shift = next_shift_edit->tosqlhtml();
    QString part_change_statue = part_change_edit->tosqlhtml();
    QString insert_query = QString("INSERT INTO EIS_document ("
                                   "`idx`,"
                                   "`team`,"
                                   "`write_time`,"
                                   "`factory_process`,"
                                   "`facilities_name`,"
                                   "`witer_name`,"
                                   "`change_have`,"
                                   "`time_part`,"
                                   "`document_name`,"
                                   "`phenomenon`,"
                                   "`cause`,"
                                   "`current_action`,"
                                   "`Lot_action_statue`,"
                                   "`change_master_sheet`,"
                                   "`next_shift_link`,"
                                   "`part_change_statue`,"
                                   "`after_LOT_monitering`,"
                                   "`downloadimg`,"
                                   "`complete`"
                                   ") "
                                   "VALUES ("
                                   "'"+QString("%1").arg(doc_number)+"',"
                                   "'"+team_name+"',"
                                   "'"+now_datetime+"',"
                                   "'"+process+"',"
                                   "'"+facilities_name+"',"
                                   "'"+write_name+"',"
                                   "'"+change_have+"',"
                                   "'"+Time_part+"',"
                                   "'"+document_name+"',"
                                   "'"+phenomenon+"',"
                                   "'"+cause+"',"
                                   "'"+current_action+"',"
                                   "'"+current_lot_action+"',"
                                   "'"+change_master_sheet+"',"
                                   "'"+next_shift+"',"
                                   "'"+part_change_statue+"',"
                                   "'"+after_lot_monitering+"',"
                                   "' ',"
                                   "'2'"
                                   ");");
        query.exec(insert_query);
        ftp->connectToHost(server_ip,21);
        ftp->login(QUrl::fromPercentEncoding("EIS"),"1234");
        ftp->cd("img/");
        ftp->mkdir(QString("%1").arg(doc_number));
        ftp->cd(QString("%1").arg(doc_number));
        QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img/"+QString("%1").arg(doc_number);
        QDir doc_dir(makedir_txt);
        QStringList filelist =  doc_dir.entryList(QDir::Files);
        for(int i=0;i<filelist.count();i++){
            qDebug()<<"filelist"<<filelist.at(i);
            QString des_file = makedir_txt+filelist.at(i);
            QFile *file = new QFile(des_file);


        }


//        qDebug()<<"query : "<<query.lastQuery();
//        qDebug()<<"error : "<<query.lastError().text();


}

void EISmain::ftpCommandFinished(int commandId, bool error)
{
    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        qDebug()<<"connection :"<<error;
    }
    if (ftp->currentCommand() == QFtp::Login){
        qDebug()<<"login : "<<error;

    }
    qDebug()<<"ftpfinish";
}

void EISmain::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    qDebug()<<"readbytes = "<<readBytes;
    qDebug()<<"totalBytes = "<<totalBytes;
}

