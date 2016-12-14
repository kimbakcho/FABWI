#include "eismain.h"
#include "ui_eismain.h"
#include "ui_eis_serarch_item.h"
#include "ui_eis_alarm_serarch_item.h"

EISmain::EISmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EISmain)
{
    ui->setupUi(this);
    progressdialog = 0;
    ui->current_time->setDateTime(QDateTime::currentDateTime());

    QTime nowtime = ui->current_time->time();
    int second_diff = nowtime.secsTo(QTime(17,00,0));
    if(second_diff>=0){
        ui->select_part_time->setCurrentIndex(0);
    }else{
        ui->select_part_time->setCurrentIndex(1);
    }

    time_update.setInterval(1000);
    time_update.start();
    connect(&time_update,SIGNAL(timeout()),this,SLOT(time_update_out()));

    alarm_loop_timer.setInterval(10000);
    alarm_loop_timer.start();
    connect(&alarm_loop_timer,SIGNAL(timeout()),this,SLOT(alarm_search_loop()));

    QString configini_str = qApp->applicationDirPath()+"/server.ini";
    ftp = new QFtp(this);

    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    server_ip = settings.value("serverip").toString();
    db_port = settings.value("dbport").toString();
    ftp_port = settings.value("ftpport").toString();
    settings.endGroup();

    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp,SIGNAL(rawCommandReply(int,QString)),
            this,SLOT(ftp_rawCommandReply(int,QString)));

    connect(ftp,SIGNAL(listInfo(QUrlInfo)),
            this,SLOT(ftp_listInfo(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));


    if(!db.contains("EISDB")){
       db = QSqlDatabase::addDatabase("QMYSQL","EISDB");
       db.setHostName(server_ip);
       db.setPort(db_port.toInt());
       db.setUserName(DBID);
       db.setPassword(DBPW);
       db.setDatabaseName("FAB");
    }else {
       db = QSqlDatabase::database("EISDB");
    }

    if(!db.isOpen()){
        if(!db.open()){
             int ret = QMessageBox::warning(this, tr("conntion false"),
                                               "server connection fail\n"
                                                  ""+db.lastError().text(),
                                                    QMessageBox::Close);
        }
    }

    //docnumber을 +1 한값을 데이터베이스 업로드 시켜 문서가 서로 중첩되지 않게한다.
    QSqlQuery query(db);

    QString str_query = QString("update EIS_management set document_number = EIS_management.document_number +1;");
    query.exec(str_query);
    QDate current_day;
    current_day = QDate::currentDate();

    ui->search_start_time->setDate(current_day.addDays(-1));
    ui->search_end_time->setDateTime(QDateTime::currentDateTime());

    query.exec("select * from EIS_management");
    query.next();
    doc_number = query.value("document_number").toInt();

    ui->select_team->addItem("");
    ui->select_team_2->addItem("");
    ui->search_select_team->addItem("");


    ui->select_name->addItem("");

    query.exec("select team from People_information GROUP by team;");
    while(query.next()){
        ui->select_team->addItem(query.value("team").toString());
        ui->select_team_2->addItem(query.value("team").toString());
        ui->search_select_team->addItem(query.value("team").toString());
    }

    QHeaderView *tempheader;
    tempheader = ui->search_listview->horizontalHeader();
    tempheader->resizeSection(3,400); //제목 사이즈 변경
    tempheader->resizeSection(2,150); //시간 사이즈 변경
    tempheader->resizeSection(1,60); //num 사이즈 변경
    tempheader->resizeSection(0,25); //체크박스 사이즈 변경
    tempheader->hideSection(10);

    QHeaderView *alarm_tempheader;
    alarm_tempheader = ui->alarm_search_listview->horizontalHeader();
    alarm_tempheader->resizeSection(3,400); //제목 사이즈 변경
    alarm_tempheader->resizeSection(2,150); //시간 사이즈 변경
    alarm_tempheader->resizeSection(1,60); //num 사이즈 변경
    alarm_tempheader->resizeSection(0,25); //체크박스 사이즈 변경
    alarm_tempheader->hideSection(0);

    connect(tempheader,SIGNAL(sectionClicked(int)),this,SLOT(header_click(int)));
    content_edit = new BTextEdit(&doc_number,this);
    content_edit->setObjectName("content_edit");
    ui->layout_content->addWidget(content_edit,0,0);

    attach_list_model = new QStandardItemModel();
    ui->attach_listview->setModel(attach_list_model);

    alarm_attach_list_model = new QStandardItemModel();
    ui->attach_listview_2->setModel(alarm_attach_list_model);

    QString font_family = ui->font_type->fontInfo().family();
    int font_size = ui->fontsize->value();
    mainfont.setFamily(font_family);
    mainfont.setPointSize(font_size);

    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(charfotmet);

    query.exec("select content_basc_form from EIS_management");
    query.next();
    content_edit->setHtml(query.value("content_basc_form").toString());

    QTextCharFormat alarm_charfotmet;
    alarm_charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(alarm_charfotmet);

    QString alarm_font_family = ui->font_type_2->fontInfo().family();
    int alarm_font_size = ui->fontsize_2->value();
    mainfont.setFamily(alarm_font_family);
    mainfont.setPointSize(alarm_font_size);

    ui->current_time_2->setDateTime(QDateTime::currentDateTime());
    str_query = QString("update EIS_management set alarm_document_number = EIS_management.alarm_document_number +1;");
    query.exec(str_query);
    query.exec("select * from EIS_management");
    query.next();


    alarm_doc_number =  query.value("alarm_document_number").toInt();
    alarm_content_edit = new BTextEdit(&alarm_doc_number,this);
    alarm_content_edit->setObjectName("alarm_content_edit");
    ui->layout_content_2->addWidget(alarm_content_edit,0,0);

    query.exec("select alarm_content_base_from from EIS_management");
    query.next();
    alarm_content_edit->setHtml(query.value("alarm_content_base_from").toString());

    ui->workstart_time->setDate(QDate::currentDate());
    ui->workend_time->setDate(QDate::currentDate());

    on_search_button_clicked();

    search_alarm_logic();



}

EISmain::~EISmain()
{
    delete ui;
}



void EISmain::header_click(int data){

}

void EISmain::time_update_out()
{
    ui->current_time->setDateTime(QDateTime::currentDateTime());
    ui->current_time_2->setDateTime(QDateTime::currentDateTime());
}

void EISmain::on_select_team_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->select_process->clear();
        ui->select_process->addItem("");
        query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process order by factory_process asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_process->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        query_txt = QString("select facilities from People_information where team = '%1' group by facilities order by facilities asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
        ui->select_name->clear();
        ui->select_name->addItem("");
        query_txt = QString("select name from People_information where team = '%1' group by name order by name asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_name->addItem(query.value("name").toString());
        }
    }else {
        ui->select_process->clear();
        ui->select_process->addItem("");
        query_txt = QString("select factory_process from People_information group by factory_process order by factory_process asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_process->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        query_txt = QString("select facilities from People_information group by facilities order by facilities asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
        ui->select_name->clear();
        ui->select_name->addItem("");
        query_txt = QString("select name from People_information group by name order by name");
        query.exec(query_txt);
        while(query.next()){
            ui->select_name->addItem(query.value("name").toString());
        }
    }
}

void EISmain::on_select_team_2_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->select_process_2->clear();
        ui->select_process_2->addItem("");
        query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process order by factory_process asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_process_2->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities_2->clear();
        ui->select_facilities_2->addItem("");
        query_txt = QString("select facilities from People_information where team = '%1' group by facilities order by facilities asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities_2->addItem(query.value("facilities").toString());
        }
        ui->select_name_2->clear();
        ui->select_name_2->addItem("");
        query_txt = QString("select name from People_information where team = '%1' group by name order by name asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_name_2->addItem(query.value("name").toString());
        }
    }else {
        ui->select_process_2->clear();
        ui->select_process_2->addItem("");
        query_txt = QString("select factory_process from People_information group by factory_process order by factory_process asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_process_2->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities_2->clear();
        ui->select_facilities_2->addItem("");
        query_txt = QString("select facilities from People_information group by facilities order by facilities asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities_2->addItem(query.value("facilities").toString());
        }
        ui->select_name_2->clear();
        ui->select_name_2->addItem("");
        query_txt = QString("select name from People_information group by name order by name");
        query.exec(query_txt);
        while(query.next()){
            ui->select_name_2->addItem(query.value("name").toString());
        }
    }
}

void EISmain::on_search_select_team_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->search_select_process->clear();
        ui->search_select_process->addItem("");
        query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process order by factory_process asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_process->addItem(query.value("factory_process").toString());
        }
        ui->search_select_facilities->clear();
        ui->search_select_facilities->addItem("");
        query_txt = QString("select facilities from People_information where team = '%1' group by facilities order by facilities asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_facilities->addItem(query.value("facilities").toString());
        }
        ui->search_select_name->clear();
        ui->search_select_name->addItem("");
        query_txt = QString("select name from People_information where team = '%1' group by name order by name asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_name->addItem(query.value("name").toString());
        }
    }else {
        ui->search_select_process->clear();
        ui->search_select_process->addItem("");
        query_txt = QString("select factory_process from People_information group by factory_process order by factory_process asc");
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_process->addItem(query.value("factory_process").toString());
        }
        ui->search_select_facilities->clear();
        ui->search_select_facilities->addItem("");
        query_txt = QString("select facilities from People_information group by facilities order by facilities asc");
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_facilities->addItem(query.value("facilities").toString());
        }
        ui->search_select_name->clear();
        ui->search_select_name->addItem("");
        query_txt = QString("select name from People_information group by name order by name");
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_name->addItem(query.value("name").toString());
        }
    }
}

void EISmain::on_select_process_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        QString search;
        if(ui->select_team->currentText()!=""){
            search = QString("team = '%1' AND").arg(ui->select_team->currentText());
        }else {
            search = "";
        }
        query_txt = QString("select facilities from People_information where %1 factory_process = '%2' "
                            "group by facilities order by facilities asc").arg(search).arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
    }else {
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        QString search;
        if(ui->select_team->currentText()!=""){
            search = QString("where team = '%1' AND").arg(ui->select_team->currentText());
        }else {
            search = "";
        }

        query_txt = QString("select facilities from People_information %1 "
                            "group by facilities order by facilities asc").arg(search);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
    }
}
void EISmain::on_select_process_2_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->select_facilities_2->clear();
        ui->select_facilities_2->addItem("");
        QString search;
        if(ui->select_team_2->currentText()!=""){
            search = QString("team = '%1' AND").arg(ui->select_team_2->currentText());
        }else {
            search = "";
        }
        query_txt = QString("select facilities from People_information where %1 factory_process = '%2' "
                            "group by facilities order by facilities asc").arg(search).arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities_2->addItem(query.value("facilities").toString());
        }
    }else {
        ui->select_facilities_2->clear();
        ui->select_facilities_2->addItem("");
        QString search;
        if(ui->select_team_2->currentText()!=""){
            search = QString("where team = '%1' AND").arg(ui->select_team_2->currentText());
        }else {
            search = "";
        }

        query_txt = QString("select facilities from People_information %1 "
                            "group by facilities order by facilities asc").arg(search);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities_2->addItem(query.value("facilities").toString());
        }
    }
}
void EISmain::on_search_select_process_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->search_select_facilities->clear();
        ui->search_select_facilities->addItem("");
        QString search;
        if(ui->search_select_team->currentText()!=""){
            search = QString("team = '%1' AND").arg(ui->search_select_team->currentText());
        }else {
            search = "";
        }
        query_txt = QString("select facilities from People_information where %1 factory_process = '%2' "
                            "group by facilities order by facilities asc").arg(search).arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_facilities->addItem(query.value("facilities").toString());
        }
    }else {
        ui->search_select_facilities->clear();
        ui->search_select_facilities->addItem("");
        QString search;
        if(ui->search_select_team->currentText()!=""){
            search = QString("where team = '%1' AND").arg(ui->search_select_team->currentText());
        }else {
            search = "";
        }
        query_txt = QString("select facilities from People_information %1 "
                            "group by facilities order by facilities asc").arg(search);
        query.exec(query_txt);
        while(query.next()){
            ui->search_select_facilities->addItem(query.value("facilities").toString());
        }
    }
}
void EISmain::on_select_facilities_currentIndexChanged(const QString &arg1)
{


}
void EISmain::on_search_select_facilities_currentIndexChanged(const QString &arg1)
{


}
void EISmain::on_add_button_clicked()
{
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);
    if(ui->select_team->currentText()==""){
        msg.setText(tr("empty team_name"));

        msg.exec();
        return ;
    }
    if(ui->select_process->currentText()==""){
        msg.setText(tr("empty process"));

        msg.exec();
        return ;
    }
    if(ui->select_facilities->currentText()==""){
        msg.setText(tr("empty facilities"));

        msg.exec();
        return ;
    }
    if(ui->select_facilities->currentText()==""){
        msg.setText(tr("empty facilities"));

        msg.exec();
        return ;
    }
    if(ui->select_name->currentText()==""){
        msg.setText(tr("empty name"));

        msg.exec();
        return ;
    }

    if(ui->document_name->text()==""){
        msg.setText(tr("empty document_name"));

        msg.exec();
        return ;
    }
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand("CWD /home/eis/img");
    loop.exec();
    ftp->rawCommand(QString("MKD %1").arg(doc_number));
    loop.exec();
    ftp->rawCommand(QString("CWD /home/eis/img/%1").arg(doc_number));
    loop.exec();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img/"+QString("%1").arg(doc_number);
    QDir doc_dir(makedir_txt);
    QStringList filelist =  doc_dir.entryList(QDir::Files);
    for(int i=0;i<filelist.count();i++){
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        QString des_file = makedir_txt+"/"+filelist.at(i);
        QFile *file = new QFile(des_file);
        ftp->put(file,filelist.at(i),QFtp::Binary);
        QString part = QString("%1/%2").arg(i).arg(filelist.count());
        progressdialog->setLabelText(part);
        progressdialog->exec();
    }

    QString now_datetime =ui->current_time->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString change_have;
    if(ui->radio_btn_change_exist->isChecked()){
        change_have = QString("%1").arg(1);
    }else {
        change_have = QString("%1").arg(0);
    }
    QSqlQuery query(db);
    QString Time_part = '1';


    if(ui->select_part_time->currentIndex()==0){
        Time_part = '0';
    }else if (ui->select_part_time->currentIndex()==1) {
        Time_part = '1';
    }
    QString document_name = ui->document_name->text();
    QString team_name = ui->select_team->currentText();
    QString process = ui->select_process->currentText();
    QString write_name = ui->select_name->currentText();
    QString facilities_name = ui->select_facilities->currentText();
    QString content_change = content_edit->tosqlhtml();

    content_change.replace(qApp->applicationDirPath(),"server_image_change_part_temp");
    QString content = content_change;

    QString attach_file_list;
    QStringList total_img_list;
    QString total_img_txt;

    for(int i=0;i<content_edit->image_list.count();i++){
        QString img_path = content_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<total_img_list.count();i++){
        QString img_file = total_img_list.at(i);
        total_img_txt = total_img_txt+img_file+"/////";
    }
    for(int i=0;i<attach_list_model->rowCount();i++){
        QString item = attach_list_model->item(i)->text();
        attach_file_list = attach_file_list + item + "/////";
    }
    QString complete =  "2";
//    if(ui->complete->isChecked()){
//        complete = "3";
//    }else {
//        complete = "2";
//    }
//    send_email();

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
                                   "`content`,"
                                   "`downloadimg`,"
                                   "`attach_file_list`,"
                                   "`complete`,"
                                   "`worktime_start`,"
                                   "`worktime_end`"
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
                                   "'"+content+"',"
                                   "'"+total_img_txt+"',"
                                   "'"+attach_file_list+"',"
                                   "'"+complete+"',"
                                   "'"+ui->workstart_time->dateTime().toString("yyyy-MM-dd HH:mm:ss")+"',"
                                   "'"+ui->workend_time->dateTime().toString("yyyy-MM-dd HH:mm:ss")+"'"
                                   ");");
        query.exec(insert_query);

        QString str_query = QString("update EIS_management set document_number = EIS_management.document_number +1;");
        query.exec(str_query);
        query.exec("select * from EIS_management");
        query.next();
        doc_number = query.value("document_number").toInt();

        QTime nowtime = ui->current_time->time();
        int second_diff = nowtime.secsTo(QTime(17,00,0));
        if(second_diff>=0){
            ui->select_part_time->setCurrentIndex(0);
        }else{
            ui->select_part_time->setCurrentIndex(1);
        }

        ui->select_team->setCurrentIndex(0);
        ui->document_name->clear();
        attach_list_model->clear();

        query.exec("select content_basc_form from EIS_management");
        query.next();
        content_edit->setHtml(query.value("content_basc_form").toString());

        content_edit->image_list.clear();

        ui->workstart_time->setDate(QDate::currentDate());\
        ui->workstart_time->setTime(QTime(0,0,0));
        ui->workend_time->setDate(QDate::currentDate());
        ui->workend_time->setTime(QTime(0,0,0));

        msg.setText(tr("add complete"));

        msg.exec();

        ui->search_end_time->setDateTime(QDateTime::currentDateTime());

        on_search_button_clicked();
}
void EISmain::on_add_button_2_clicked()
{
    QMessageBox msg;
    msg.addButton(QMessageBox::Ok);

    if(ui->document_name_2->text()==""){
        msg.setText(tr("empty document_name"));

        msg.exec();
        return ;
    }
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand("CWD /home/eis/alarmimg");
    loop.exec();
    ftp->rawCommand(QString("MKD %1").arg(alarm_doc_number));
    loop.exec();
    ftp->rawCommand(QString("CWD /home/eis/alarmimg/%1").arg(alarm_doc_number));
    loop.exec();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/alarmimg/"+QString("%1").arg(alarm_doc_number);
    QDir doc_dir(makedir_txt);
    QStringList filelist =  doc_dir.entryList(QDir::Files);
    for(int i=0;i<filelist.count();i++){
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        QString des_file = makedir_txt+"/"+filelist.at(i);
        QFile *file = new QFile(des_file);
        ftp->put(file,filelist.at(i),QFtp::Binary);
        QString part = QString("%1/%2").arg(i).arg(filelist.count());
        progressdialog->setLabelText(part);
        progressdialog->exec();
    }
    QString now_datetime =ui->current_time->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QSqlQuery query(db);

    QString document_name = ui->document_name_2->text();
    QString team_name = ui->select_team_2->currentText();
    QString process = ui->select_process_2->currentText();
    QString write_name = ui->select_name_2->currentText();
    QString facilities_name = ui->select_facilities_2->currentText();
    QString content_change = alarm_content_edit->tosqlhtml();

    content_change.replace(qApp->applicationDirPath(),"server_image_change_part_temp");
    QString content = content_change;

    QString attach_file_list;
    QStringList total_img_list;
    QString total_img_txt;

    for(int i=0;i<alarm_content_edit->image_list.count();i++){
        QString img_path = alarm_content_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<total_img_list.count();i++){
        QString img_file = total_img_list.at(i);
        total_img_txt = total_img_txt+img_file+"/////";
    }
    for(int i=0;i<alarm_attach_list_model->rowCount();i++){
        QString item = alarm_attach_list_model->item(i)->text();
        attach_file_list = attach_file_list + item + "/////";
    }
    QString priority_value;
    if(ui->priority_low->isChecked()){
        priority_value = "1";
    }else if(ui->priority_middle->isChecked()){
        priority_value = "2";
    }else if(ui->priority_high->isChecked()){
        priority_value = "3";
    }else if(ui->priority_very_high->isChecked()){
        priority_value = "4";
    }

    QString insert_query = QString("INSERT INTO EIS_alarm_document ("
                           "`idx`, "
                           "`team`, "
                           "`write_time`, "
                           "`factory_process`, "
                           "`facilities_name`, "
                           "`witer_name`, "
                           "`document_name`, "
                           "`content`, "
                           "`downloadimg`, "
                           "`attach_file_list`,"
                           "`priority`"
                           ") "
                           "VALUES ("
                           "'"+QString("%1").arg(alarm_doc_number)+"',"
                           "'"+team_name+"',"
                           "'"+now_datetime+"',"
                           "'"+process+"',"
                           "'"+facilities_name+"',"
                           "'"+write_name+"',"
                           "'"+document_name+"',"
                           "'"+content+"',"
                           "'"+total_img_txt+"',"
                           "'"+attach_file_list+"',"
                           "'"+priority_value+"'"
                           ")");
        query.exec(insert_query);

        QString str_query = QString("update EIS_management set alarm_document_number = EIS_management.alarm_document_number +1;");
        query.exec(str_query);
        query.exec("select * from EIS_management");
        query.next();
        alarm_doc_number = query.value("alarm_document_number").toInt();

        ui->document_name_2->clear();
        alarm_attach_list_model->clear();

        query.exec("select alarm_content_base_from from EIS_management");
        query.next();
        alarm_content_edit->setHtml(query.value("alarm_content_base_from").toString());

        ui->priority_middle->setChecked(true);

        alarm_content_edit->image_list.clear();




        msg.setText(tr("add complete1"));

        msg.exec();

        ui->search_end_time->setDateTime(QDateTime::currentDateTime());

        search_alarm_logic();

}
void EISmain::ftpCommandFinished(int commandId, bool error)
{

    loop.exit();
}

void EISmain::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    progressdialog->setMaximum(totalBytes);
    progressdialog->setValue(readBytes);
}

void EISmain::ftp_listInfo(QUrlInfo urlInfo)
{
    qDebug()<<urlInfo.name();
}

void EISmain::ftp_rawCommandReply(int a, QString data)
{
    //    qDebug()<<"ftp data = "<<data;
}

void EISmain::send_email()
{
    SmtpClient smtp("mx.info.wisol.co.kr", 25, SmtpClient::TcpConnection);

    MimeMessage message;

    EmailAddress sender("bhkim@wisol.co.kr", "beckho");
    message.setSender(&sender);

    EmailAddress to("bhkim@wisol.co.kr", "beckho");
    message.addRecipient(&to);

    message.setSubject("SmtpClient for Qt - Demo");
    MimeHtml html;

    QTextEdit edit;

    QTextTable *table1 = edit.textCursor().insertTable(2,6);

    table1->cellAt(0,0).firstCursorPosition().insertText(tr("team"));
    table1->cellAt(1,0).firstCursorPosition().insertText(ui->select_team->currentText());
    table1->cellAt(0,1).firstCursorPosition().insertText(tr("process"));
    table1->cellAt(1,1).firstCursorPosition().insertText(ui->select_process->currentText());
    table1->cellAt(0,2).firstCursorPosition().insertText(tr("facilities"));
    table1->cellAt(1,2).firstCursorPosition().insertText(ui->select_facilities->currentText());
    table1->cellAt(0,3).firstCursorPosition().insertText(tr("witer_name"));
    table1->cellAt(1,3).firstCursorPosition().insertText(ui->select_name->currentText());
    table1->cellAt(0,4).firstCursorPosition().insertText(tr("change_have"));
    if(ui->radio_btn_change_exist->isChecked()){
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("have"));
    }else {
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("don't have"));
    }
    table1->cellAt(0,5).firstCursorPosition().insertText(tr("complete"));
//    if(ui->complete->isChecked()){
//        table1->cellAt(1,5).firstCursorPosition().insertText(tr("have"));
//    }else {
//        table1->cellAt(1,5).firstCursorPosition().insertText(tr("don't have"));
//    }
    edit.textCursor().insertText("\n");

    QTextTable *table2 = edit.textCursor().insertTable(1,2);
    table2->cellAt(0,0).firstCursorPosition().insertText(tr("subname"));
    table2->cellAt(0,1).firstCursorPosition().insertText(ui->document_name->text());

    QString send_mail_html  = content_edit->toHtml();

    QString change_image_url_before = qApp->applicationDirPath()+"/temp/EIS/img";
    QString change_image_url_After = "http://10.20.10.230/img";

    send_mail_html.replace("file:///"+change_image_url_before,change_image_url_After);
//    qDebug()<<send_mail_html;
    edit.textCursor().insertHtml(send_mail_html);

    html.setHtml(edit.toHtml());

    message.addPart(&html);
    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
    }
    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;

    }
    smtp.quit();

}
void EISmain::alarm_search_loop()
{
    search_alarm_logic();
}

void EISmain::modify_complete()
{

    ui->search_end_time->setDateTime(QDateTime::currentDateTime());

    on_search_button_clicked();
}



void EISmain::on_attach_btn_clicked()
{
    QFileDialog filedialog(this);
    filedialog.setFileMode(QFileDialog::AnyFile);
    filedialog.setViewMode(QFileDialog::Detail);
    if(filedialog.exec()==QFileDialog::Accepted){
        QString filename = filedialog.selectedFiles().at(0);
        if(ftp->state()==QFtp::Unconnected){
            ftp->connectToHost(server_ip,21);

            ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
            loop.exec();
            ftp->setTransferMode(QFtp::Passive);
        }
        ftp->rawCommand("CWD /home/eis/attach");
        loop.exec();
        ftp->rawCommand(QString("MKD %1").arg(doc_number));
        loop.exec();
        ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
        loop.exec();
        QFile *source_file = new QFile(filename);
        source_file->open(QFile::ReadWrite);
        QString upload_filename = filename.split('/').last();
        for(int i=0;i<attach_list_model->rowCount();i++){
              QString item = attach_list_model->item(i)->text();
              if(item == upload_filename){
                    QString first_name = upload_filename.split(".").at(0)+"_t";
                    QString Last_name;
                    if(upload_filename.split('.').size()>=2){
                        Last_name = "."+upload_filename.split(".").at(1);
                    }
                    upload_filename =first_name + Last_name;
              }
        }
        ftp->put(source_file,upload_filename);
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        progressdialog->setLabelText("upload");
        progressdialog->exec();
        attach_list_model->insertRow(attach_list_model->rowCount(),
                                     new QStandardItem(upload_filename));
        source_file->close();
    }
}

void EISmain::on_attach_btn_2_clicked()
{
    QFileDialog filedialog(this);
    filedialog.setFileMode(QFileDialog::AnyFile);
    filedialog.setViewMode(QFileDialog::Detail);
    if(filedialog.exec()==QFileDialog::Accepted){
        QString filename = filedialog.selectedFiles().at(0);

        if(ftp->state()==QFtp::Unconnected){
            ftp->connectToHost(server_ip,21);

            ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
            loop.exec();
            ftp->setTransferMode(QFtp::Passive);
        }

        ftp->rawCommand("CWD /home/eis/alarmattach");
        loop.exec();
        ftp->rawCommand(QString("MKD %1").arg(alarm_doc_number));
        loop.exec();
        ftp->rawCommand(QString("CWD /home/eis/alarmattach/%1").arg(alarm_doc_number));
        loop.exec();
        QFile *source_file = new QFile(filename);
        source_file->open(QFile::ReadWrite);
        QString upload_filename = filename.split('/').last();
        for(int i=0;i<alarm_attach_list_model->rowCount();i++){
              QString item = alarm_attach_list_model->item(i)->text();
              if(item == upload_filename){
                    QString first_name = upload_filename.split(".").at(0)+"_t";
                    QString Last_name;
                    if(upload_filename.split('.').size()>=2){
                        Last_name = "."+upload_filename.split(".").at(1);
                    }
                    upload_filename =first_name + Last_name;
              }
        }

        ftp->put(source_file,upload_filename);
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        progressdialog->setLabelText("upload");
        progressdialog->exec();
        alarm_attach_list_model->insertRow(alarm_attach_list_model->rowCount(),
                                     new QStandardItem(upload_filename));
        source_file->close();
    }
}


void EISmain::on_attach_remove_btn_clicked()
{
     QModelIndex index  = ui->attach_listview->currentIndex();
     QString select_file = index.data().toString();
     if(ftp->state()==QFtp::Unconnected){
         ftp->connectToHost(server_ip,21);

         ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
         loop.exec();
         ftp->setTransferMode(QFtp::Passive);
     }
     ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
     loop.exec();
     ftp->remove(select_file);
     loop.exec();
     attach_list_model->removeRow(index.row());
}


void EISmain::on_attach_remove_btn_2_clicked()
{
    QModelIndex index  = ui->attach_listview_2->currentIndex();
    QString select_file = index.data().toString();
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/alarmattach/%1").arg(alarm_doc_number));
    loop.exec();
    ftp->remove(select_file);
    loop.exec();
    alarm_attach_list_model->removeRow(index.row());
}


void EISmain::on_attach_listview_doubleClicked(const QModelIndex &index)
{
    QFileDialog file_dialog;
    QString source_file = index.data().toString();
    QString save_filepath = file_dialog.getSaveFileName(this,tr("Save File"),index.data().toString());
    qDebug()<<"save_filepath = "<<save_filepath;
    if(save_filepath==""){
        return;
    }
    QFile *save_file = new QFile(save_filepath);
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
    loop.exec();
    save_file->open(QFile::ReadWrite);
    ftp->get(source_file,save_file);
    if(progressdialog == 0){
        progressdialog = new QProgressDialog(this);
    }
    progressdialog->setLabelText("download");
    progressdialog->exec();
    save_file->close();
}

void EISmain::on_attach_listview_2_doubleClicked(const QModelIndex &index)
{
    QFileDialog file_dialog;
    QString source_file = index.data().toString();
    QString save_filepath = file_dialog.getSaveFileName(this,tr("Save File"),index.data().toString());
    qDebug()<<"save_filepath = "<<save_filepath;
    if(save_filepath==""){
        return;
    }
    QFile *save_file = new QFile(save_filepath);
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/alarmattach/%1").arg(alarm_doc_number));
    loop.exec();
    save_file->open(QFile::ReadWrite);
    ftp->get(source_file,save_file);
    if(progressdialog == 0){
        progressdialog = new QProgressDialog(this);
    }
    progressdialog->setLabelText("download");
    progressdialog->exec();
    save_file->close();
}



void EISmain::on_fontsize_editingFinished()
{
    QFont update_font;
    update_font = content_edit->currentCharFormat().font();
    update_font.setPointSize(ui->fontsize->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}
void EISmain::on_fontsize_2_editingFinished()
{
    QFont update_font;
    update_font = alarm_content_edit->currentCharFormat().font();
    update_font.setPointSize(ui->fontsize_2->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(alarm_content_edit->currentCharFormat().foreground());
    alarm_content_edit->setCurrentCharFormat(charfotmet);
}
void EISmain::on_font_type_currentTextChanged(const QString &arg1)
{
    QFont update_font;
    update_font = content_edit->currentCharFormat().font();
    update_font.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}

void EISmain::on_font_type_2_currentTextChanged(const QString &arg1)
{
    QFont update_font;
    update_font = alarm_content_edit->currentCharFormat().font();
    update_font.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(alarm_content_edit->currentCharFormat().foreground());
    alarm_content_edit->setCurrentCharFormat(charfotmet);
}
void EISmain::on_content_big_view_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->content_label->text(),content_edit,ui->font_type,
                                              ui->fontsize,ui->Bold_btn,ui->underline_btn,
                                              ui->now_color,ui->color_dialog,
                                              ui->layout_content,ui->font_layout);
    big_view->show();
}


void EISmain::on_content_big_view_2_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->content_label_2->text(),alarm_content_edit,ui->font_type_2,
                                              ui->fontsize_2,ui->Bold_btn_2,ui->underline_btn_2,
                                              ui->now_color_2,ui->color_dialog_2,
                                              ui->layout_content_2,ui->font_layout_2);
    big_view->show();
}


void EISmain::on_search_button_clicked()
{
    QSqlQuery query(db);
    QString query_txt = Return_search_query();
    query_txt.append(" ORDER BY `write_time` DESC");
    query.exec(query_txt);
    QVector<int> idxlist;
    while(ui->search_listview->rowCount()!=0){
        ui->search_listview->removeRow(ui->search_listview->rowCount()-1);
    }
    for(int i=0;i<itemlist.size();i++){
        itemlist.at(i)->deleteLater();
    }
    itemlist.clear();
    while(query.next()){

        int rowcount = ui->search_listview->rowCount();
        EIS_serarch_item *item =new EIS_serarch_item;

        if(idxlist.contains(query.value("idx").toInt())){
           continue;
        }else {
            idxlist.append(query.value("idx").toInt());
        }
        ui->search_listview->insertRow(rowcount);
        ui->search_listview->setCellWidget(rowcount,0,item->ui->select_doc);

        item->ui->doc_number->setText(QString("%1").arg(query.value("idx").toString()));
        ui->search_listview->setCellWidget(rowcount,1,item->ui->doc_number);

        item->ui->write_time->setText(QString("%1").arg(query.value("write_time")
                                                        .toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        ui->search_listview->setCellWidget(rowcount,2,item->ui->write_time);

        item->ui->doc_name->setText(QString("%1").arg(query.value("document_name").toString()));
        ui->search_listview->setCellWidget(rowcount,3,item->ui->doc_name);

        item->ui->team->setText(QString("%1").arg(query.value("team").toString()));
        ui->search_listview->setCellWidget(rowcount,4,item->ui->team);

        item->ui->process->setText(QString("%1").arg(query.value("factory_process").toString()));
        ui->search_listview->setCellWidget(rowcount,5,item->ui->process);

        item->ui->facilities->setText(QString("%1").arg(query.value("facilities_name").toString()));
        ui->search_listview->setCellWidget(rowcount,6,item->ui->facilities);

        item->ui->write_name->setText(QString("%1").arg(query.value("witer_name").toString()));
        ui->search_listview->setCellWidget(rowcount,7,item->ui->write_name);

        if(query.value("change_have").toInt() == 1){
            item->ui->change_have->setText(tr("have"));
        }else {
            item->ui->change_have->setText(tr("don't have"));
        }
        ui->search_listview->setCellWidget(rowcount,8,item->ui->change_have);

        if(query.value("time_part").toInt() == 0){
            item->ui->time_part->setText(tr("have sun"));
        }else {
            item->ui->time_part->setText(tr("don't have sun"));
        }
        ui->search_listview->setCellWidget(rowcount,9,item->ui->time_part);

        if(query.value("complete").toInt() == 3){
            item->ui->chbox_complete->setChecked(true);
        }else {
            item->ui->chbox_complete->setChecked(false);
        }
        ui->search_listview->setCellWidget(rowcount,10,item->ui->chbox_complete);

        itemlist.append(item);
    }

    if(itemlist.size()==0){
        QMessageBox msg;
        msg.setText(tr("no reslut"));
        msg.addButton(QMessageBox::Ok);
        msg.exec();
    }
}
void EISmain::search_alarm_logic()
{
    QSqlQuery query(db);
    QVector<int> idxlist;
    while(ui->alarm_search_listview->rowCount()!=0){
        ui->alarm_search_listview->removeRow(ui->alarm_search_listview->rowCount()-1);
    }
    for(int i=0;i<alarm_itemlist.size();i++){
        alarm_itemlist.at(i)->deleteLater();
    }
    alarm_itemlist.clear();
    if(!ui->alarm_search_apply->isChecked()){
        QString query_txt = "select * from EIS_alarm_document where delete_have = 0 order by priority desc, write_time desc";
        query.exec(query_txt);
        while(query.next()){
            int rowcount = ui->alarm_search_listview->rowCount();
            eis_alarm_serarch_item *item =new eis_alarm_serarch_item();
            if(idxlist.contains(query.value("idx").toInt())){
               continue;
            }else {
                idxlist.append(query.value("idx").toInt());
            }

            ui->alarm_search_listview->insertRow(rowcount);
            ui->alarm_search_listview->setCellWidget(rowcount,0,item->ui->select_doc);

            item->ui->doc_number->setText(QString("%1").arg(query.value("idx").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,1,item->ui->doc_number);

            item->ui->write_time->setText(QString("%1").arg(query.value("write_time")
                                                            .toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            ui->alarm_search_listview->setCellWidget(rowcount,2,item->ui->write_time);

            item->ui->doc_name->setText(QString("%1").arg(query.value("document_name").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,3,item->ui->doc_name);

            item->ui->team->setText(QString("%1").arg(query.value("team").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,4,item->ui->team);

            item->ui->process->setText(QString("%1").arg(query.value("factory_process").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,5,item->ui->process);

            item->ui->facilities->setText(QString("%1").arg(query.value("facilities_name").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,6,item->ui->facilities);

            item->ui->write_name->setText(QString("%1").arg(query.value("witer_name").toString()));
            ui->alarm_search_listview->setCellWidget(rowcount,7,item->ui->write_name);



            QString priority_str = "";
            if(query.value("priority").toInt() == 1){
                priority_str = tr("low");
            }else if(query.value("priority").toInt() == 2){
                priority_str = tr("middle");
            }else if(query.value("priority").toInt() == 3){
                priority_str = tr("high");
            }else if(query.value("priority").toInt() == 4){
                priority_str = tr("very_high");
            }
            item->ui->priority->setText(QString("%1").arg(priority_str));
            ui->alarm_search_listview->setCellWidget(rowcount,8,item->ui->priority);



            alarm_itemlist.append(item);
        }
    }

}

void EISmain::on_search_listview_cellDoubleClicked(int row, int column)
{
    QStringList check_list;
    QString number_doc = itemlist.at(row)->ui->doc_number->text();
    number_doc = number_doc + "/"+itemlist.at(row)->ui->write_time->text();
    check_list.append(number_doc);
    Eis_list_view *list_view = new Eis_list_view(check_list);
    connect(list_view,SIGNAL(modify_button_click_pass()),this,SLOT(modify_complete()));
    list_view->modify_button_show(false);
    list_view->attach_button_show(false);
    list_view->show();
}

void EISmain::on_alarm_search_listview_cellDoubleClicked(int row, int column)
{
    QStringList check_list;
    QLabel *alarm_doc_number_label =(QLabel *)ui->alarm_search_listview->cellWidget(row,1);
    QLabel *alarm_wirte_time_label =(QLabel *)ui->alarm_search_listview->cellWidget(row,2);
    QString number_doc =alarm_doc_number_label->text();
    QString write_time = alarm_wirte_time_label->text();
    number_doc = number_doc+"/"+write_time;
    check_list.append(number_doc);
    eis_alarmlistview_item *view = new eis_alarmlistview_item(number_doc);
    connect(view,SIGNAL(update_alarm_data()),this,SLOT(alarm_search_loop()));
    view->show();

}

QString EISmain::Return_search_query()
{
    QString start_time = ui->search_start_time->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString end_time = ui->search_end_time->dateTime().toString("yyyy-MM-dd hh:mm:ss");;
    QString query_txt = QString("select * from EIS_document where write_time BETWEEN \'%1\' AND \'%2\'").arg(start_time).arg(end_time);
    if(ui->search_select_team->currentText()!=""){
        query_txt.append(QString(" AND team = \'%1\'").arg(ui->search_select_team->currentText()));
    }
    if(ui->search_select_process->currentText()!=""){
        query_txt.append(QString(" AND factory_process = \'%1\'").arg(ui->search_select_process->currentText()));
    }
    if(ui->search_select_facilities->currentText()!=""){
        query_txt.append(QString(" AND facilities_name = \'%1\'").arg(ui->search_select_facilities->currentText()));
    }
    if(ui->search_select_name->currentText()!=""){
        query_txt.append(QString(" AND witer_name LIkE \'%%1%\'").arg(ui->search_select_name->currentText()));
    }
    if(ui->search_change_have->currentText()!=""){
        if(ui->search_change_have->currentText()==tr("O")){
            query_txt.append(QString(" AND change_have = \'%1\'").arg(1));
        }else if(ui->search_change_have->currentText()==tr("X")){
            query_txt.append(QString(" AND change_have = \'%1\'").arg(0));
        }
    }
    if(ui->search_part_time->currentText()!=""){
      if(ui->search_part_time->currentIndex()==1){
         query_txt.append(QString(" AND time_part = \'%1\'").arg(0));
      }else if(ui->search_part_time->currentIndex()==2){
         query_txt.append(QString(" AND time_part = \'%1\'").arg(1));
      }
    }
    if(ui->search_name_edit->text()!=""){
        query_txt.append(QString(" AND document_name LIKE \'%%1%\'").arg(ui->search_name_edit->text()));
    }
    query_txt.append(" AND delete_have = '0'");
    return query_txt;

}



void EISmain::on_total_view_btn_clicked()
{
    QStringList check_list;

    for(int i=0;i<itemlist.count();i++){
        if(itemlist.at(i)->ui->select_doc->isChecked()){
            QString number_doc = itemlist.at(i)->ui->doc_number->text();
            number_doc = number_doc + "/"+itemlist.at(i)->ui->write_time->text();
            check_list.append(number_doc);
        }
    }
    Eis_list_view *list_view = new Eis_list_view(check_list);
    connect(list_view,SIGNAL(modify_button_click_pass()),this,SLOT(modify_complete()));
    list_view->modify_button_show(false);
    list_view->attach_button_show(false);
    list_view->show();
}

void EISmain::on_print_btn_clicked()
{
    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    printer.setFullPage(true);

    QPrintDialog *dialog= new QPrintDialog(&printer);
    int result = dialog->exec();
    if(result != QPrintDialog::Accepted){
        return;
    }

    QTextEdit edit;

    QTextTable *table1 = edit.textCursor().insertTable(2,6);

    table1->cellAt(0,0).firstCursorPosition().insertText(tr("team"));
    table1->cellAt(1,0).firstCursorPosition().insertText(ui->select_team->currentText());
    table1->cellAt(0,1).firstCursorPosition().insertText(tr("process"));
    table1->cellAt(1,1).firstCursorPosition().insertText(ui->select_process->currentText());
    table1->cellAt(0,2).firstCursorPosition().insertText(tr("facilities"));
    table1->cellAt(1,2).firstCursorPosition().insertText(ui->select_facilities->currentText());
    table1->cellAt(0,3).firstCursorPosition().insertText(tr("witer_name"));
    table1->cellAt(1,3).firstCursorPosition().insertText(ui->select_name->currentText());
    table1->cellAt(0,4).firstCursorPosition().insertText(tr("change_have"));
    if(ui->radio_btn_change_exist->isChecked()){
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("have"));
    }else {
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("don't have"));
    }
    table1->cellAt(0,5).firstCursorPosition().insertText(tr("complete"));
//    if(ui->complete->isChecked()){
//        table1->cellAt(1,5).firstCursorPosition().insertText(tr("have"));
//    }else {
//        table1->cellAt(1,5).firstCursorPosition().insertText(tr("don't have"));
//    }
    edit.textCursor().insertText("\n");

    QTextTable *table2 = edit.textCursor().insertTable(1,2);
    table2->cellAt(0,0).firstCursorPosition().insertText(tr("subname"));
    table2->cellAt(0,1).firstCursorPosition().insertText(ui->document_name->text());

    edit.textCursor().insertHtml(content_edit->toHtml());
    edit.print(&printer);

    dialog->deleteLater();
}

void EISmain::on_modify_btn_clicked()
{
    int row = ui->search_listview->selectionModel()->selectedIndexes().first().row();
    QStringList check_list;
    QString number_doc = itemlist.at(row)->ui->doc_number->text();
    number_doc = number_doc + "/"+itemlist.at(row)->ui->write_time->text();
    check_list.append(number_doc);
    Eis_list_view *list_view = new Eis_list_view(check_list);
    connect(list_view,SIGNAL(modify_button_click_pass()),this,SLOT(modify_complete()));
    list_view->show();
    list_view->worktime_readonly_mode(false);

}

void EISmain::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "EIS",
                                                                 tr("Are you sure?\n"),
                                                                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                 QMessageBox::Yes);
     if (resBtn != QMessageBox::Yes) {
         event->ignore();
     } else {
         event->accept();
         progressdialog->deleteLater();
         attach_list_model->deleteLater();
         ftp->deleteLater();
         content_edit->deleteLater();
     }
}

void EISmain::on_Bold_btn_clicked()
{
    QString current_stylesheet =  ui->Bold_btn->styleSheet();
    bool current_bold;
    if(current_stylesheet.indexOf("Bold_btn_activity.png")>0){
        current_bold = true;
    }else {
        current_bold = false;
    }
    QFont update_font ;
    if(current_bold){

        update_font = content_edit->currentCharFormat().font();
        update_font.setBold(false);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                    "}"));
    }else {

        update_font = content_edit->currentCharFormat().font();
        update_font.setBold(true);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}
void EISmain::on_Bold_btn_2_clicked()
{
    QString current_stylesheet =  ui->Bold_btn_2->styleSheet();
    bool current_bold;
    if(current_stylesheet.indexOf("Bold_btn_activity.png")>0){
        current_bold = true;
    }else {
        current_bold = false;
    }
    QFont update_font ;
    if(current_bold){

        update_font = alarm_content_edit->currentCharFormat().font();
        update_font.setBold(false);
        ui->Bold_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                    "}"));
    }else {

        update_font = alarm_content_edit->currentCharFormat().font();
        update_font.setBold(true);
        ui->Bold_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(alarm_content_edit->currentCharFormat().foreground());
    alarm_content_edit->setCurrentCharFormat(charfotmet);
}

void EISmain::on_underline_btn_clicked()
{
    QString current_stylesheet =  ui->underline_btn->styleSheet();
    bool current_underline;
    if(current_stylesheet.indexOf("under_line_activity.png")>0){
        current_underline = true;
    }else {
        current_underline = false;
    }
    QFont update_font ;
    if(current_underline){
        update_font = content_edit->currentCharFormat().font();
        update_font.setUnderline(false);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                    "}"));
    }else {
        update_font = content_edit->currentCharFormat().font();
        update_font.setUnderline(true);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}

void EISmain::on_underline_btn_2_clicked()
{
    QString current_stylesheet =  ui->underline_btn_2->styleSheet();
    bool current_underline;
    if(current_stylesheet.indexOf("under_line_activity.png")>0){
        current_underline = true;
    }else {
        current_underline = false;
    }
    QFont update_font ;
    if(current_underline){
        update_font = alarm_content_edit->currentCharFormat().font();
        update_font.setUnderline(false);
        ui->underline_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                    "}"));
    }else {
        update_font = alarm_content_edit->currentCharFormat().font();
        update_font.setUnderline(true);
        ui->underline_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(alarm_content_edit->currentCharFormat().foreground());
    alarm_content_edit->setCurrentCharFormat(charfotmet);
}


void EISmain::on_color_dialog_clicked()
{
   QColor font_color = QColorDialog::getColor();
   QFont update_font;
   update_font = content_edit->currentCharFormat().font();
   QTextCharFormat charfotmet;
   charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
   charfotmet.setForeground(QBrush(font_color));
   content_edit->setCurrentCharFormat(charfotmet);
}

void EISmain::on_color_dialog_2_clicked()
{
    QColor font_color = QColorDialog::getColor();
    QFont update_font;
    update_font = alarm_content_edit->currentCharFormat().font();
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(QBrush(font_color));
    alarm_content_edit->setCurrentCharFormat(charfotmet);
}


void EISmain::on_tabWidget_currentChanged(int index)
{

}



void EISmain::on_delete_btn_clicked()
{
    EIS_delete_dialog dialog;
    int reslut = dialog.exec();
    if(reslut == QDialog::Accepted && dialog.getPassword_alright()){
        int row = ui->search_listview->selectionModel()->selectedIndexes().first().row();
        QStringList check_list;
        QString number_doc = itemlist.at(row)->ui->doc_number->text();
        QSqlQuery query(db);
        query.exec(QString("UPDATE `EIS_document` SET `delete_have`='1' where `idx`= %1").arg(number_doc));
        ui->search_listview->removeRow(row);
        QMessageBox msg;
        msg.addButton(QMessageBox::Ok);
        msg.setText(tr("delete complete"));
        msg.exec();
    }else {

    }
}

void EISmain::on_total_delete_btn_clicked()
{
    EIS_delete_dialog dialog;
    int reslut = dialog.exec();
    if(reslut == QDialog::Accepted && dialog.getPassword_alright()){
        for(int i=0;i<itemlist.count();i++){
            if(itemlist.at(i)->ui->select_doc->isChecked()){
                QString number_doc = itemlist.at(i)->ui->doc_number->text();
                QSqlQuery query(db);
                query.exec(QString("UPDATE `EIS_document` SET `delete_have`='1' where `idx`= %1").arg(number_doc));
            }
        }
        on_search_button_clicked();
        QMessageBox msg;
        msg.addButton(QMessageBox::Ok);
        msg.setText(tr("delete complete"));
        msg.exec();
    }else {

    }
}

void EISmain::on_add_EIS_Information_btn_clicked()
{
    EIS_delete_dialog dialog;
    int reslut = dialog.exec();
    if(reslut == QDialog::Accepted && dialog.getPassword_alright()){
        EIS_input_information dialog;
        dialog.exec();
    }else {

    }
}
